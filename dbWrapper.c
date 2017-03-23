
#include <Python.h>
#include "dbAccess.h"


/* to be called by python */
static PyObject* db_getUserStreams(PyObject *self, PyObject *args);

static PyObject* db_getStreamPosts(PyObject *self, PyObject *args);

static PyObject* db_addAuthor(PyObject *self, PyObject *args);

static PyObject* db_removeAuthor(PyObject *self, PyObject *args);

static PyObject* db_newStream(PyObject *self, PyObject *args);

static PyObject* db_markAll(PyObject *self, PyObject *args);

static PyObject* db_markOne(PyObject *self, PyObject *args);





/* internal */
static PyObject* genResult_streams(SQL_result* result);

static PyObject* genResult_posts(SQL_result* result, int numRead);






static PyMethodDef dbwrapper_funcs[] =
{
	{ "getStreamPosts", (PyCFunction)db_getStreamPosts, METH_VARARGS, NULL },
	{ "getUsersStreams", (PyCFunction)db_getUserStreams, METH_VARARGS, NULL },
	{ "addAuthor", (PyCFunction)db_addAuthor, METH_VARARGS, NULL },
	{ "removeAuthor", (PyCFunction)db_removeAuthor, METH_VARARGS, NULL },
	{ "addStream", (PyCFunction)db_newStream, METH_VARARGS, NULL },
	{ "markAll", (PyCFunction)db_markAll, METH_VARARGS, NULL },
	{ "markOne", (PyCFunction)db_markOne, METH_VARARGS, NULL },
	{ NULL, NULL, 0, NULL }
};



static struct PyModuleDef db_module =
{
	PyModuleDef_HEAD_INIT,
	"dbwrapper",
	NULL,
	-1,
	dbwrapper_funcs
};



PyMODINIT_FUNC PyInit_dbwrapper(void)
{
	Py_Initialize();
	return PyModule_Create(&db_module);
}



static PyObject* db_getUserStreams(PyObject *self, PyObject *args)
{
	MYSQL* mysql;
	SQL_result* result;

	char* userID;

	userID = NULL;

	if (PyArg_ParseTuple(args, "s", &userID) == 0)
	{
		return NULL;
	}


	mysql = initSQL();

	/* get the data */
	result = getUserStreams_DB(mysql, userID);

	mysql_close(mysql);

	return genResult_streams(result);
}

static PyObject* db_getStreamPosts(PyObject *self, PyObject *args)
{
	MYSQL* mysql;
	SQL_result* result;
	char* stream;
	char* userID;
	int numRead;

	userID = NULL;
	stream = NULL;

	if (PyArg_ParseTuple(args, "ss", &userID, &stream) == 0)
	{
		return NULL;
	}

	mysql = initSQL();

	numRead = getNumRead_DB(mysql, stream, userID);

	/* get the data */
	result = getStreamPosts_DB(mysql, stream);

	mysql_close(mysql);


	return genResult_posts(result, numRead);
}



static PyObject* db_addAuthor(PyObject *self, PyObject *args)
{
	MYSQL* mysql;
	char* stream;
	char* userID;
	int status;


	stream = NULL;
	userID = NULL;

	if (PyArg_ParseTuple(args, "ss", &userID, &stream) == 0)
	{
		return NULL;
	}

	mysql = initSQL();

	/* create the new stream */
	status = newStream_DB(mysql, stream);
	if (status != 0)
	{
		mysql_close(mysql);
		return Py_BuildValue("i", status);
	}

	/* add the user to the if they were not already there */
	status = addUser_DB(mysql, stream, userID);
	mysql_close(mysql);

	return Py_BuildValue("i", status);
}


static PyObject* db_removeAuthor(PyObject *self, PyObject *args)
{
	MYSQL* mysql;
	char* stream;
	char* userID;
	int status;


	stream = NULL;
	userID = NULL;

	if (PyArg_ParseTuple(args, "ss", &userID, &stream) == 0)
	{
		return NULL;
	}

	mysql = initSQL();

	/* add the user to the if they were not already there */
	status = removeUser_DB(mysql, stream, userID);

	mysql_close(mysql);

	return Py_BuildValue("i", status);
}

static PyObject* db_markAll(PyObject *self, PyObject *args)
{
	MYSQL* mysql;
	char* stream;
	char* userID;
	int status;


	stream = NULL;
	userID = NULL;

	if (PyArg_ParseTuple(args, "ss", &userID, &stream) == 0)
	{
		return NULL;
	}

	mysql = initSQL();

	/* add the user to the if they were not already there */
	status = markAllRead_DB(mysql, stream, userID);

	mysql_close(mysql);

	return Py_BuildValue("i", status);
}

static PyObject* db_markOne(PyObject *self, PyObject *args)
{
	MYSQL* mysql;
	char* stream;
	char* userID;
	int status;


	stream = NULL;
	userID = NULL;

	if (PyArg_ParseTuple(args, "ss", &userID, &stream) == 0)
	{
		return NULL;
	}

	mysql = initSQL();

	/* add the user to the if they were not already there */
	status = markOneRead_DB(mysql, stream, userID);

	mysql_close(mysql);

	return Py_BuildValue("i", status);
}

static PyObject* db_newStream(PyObject *self, PyObject *args)
{
	MYSQL* mysql;
	char* stream;
	int status;


	stream = NULL;

	if (PyArg_ParseTuple(args, "s", &stream) == 0)
	{
		return NULL;
	}

	mysql = initSQL();

	/* add the user to the if they were not already there */
	status = newStream_DB(mysql, stream);

	mysql_close(mysql);

	return Py_BuildValue("i", status);
}



/* convert the struct into a py object and return that */

/* destroys the result struct */
static PyObject* genResult_streams(SQL_result* result)
{
	int i;
	SQL_users_result* users;
	PyObject* obj;
	PyObject* list;

	if (result == NULL)
	{
		Py_RETURN_NONE;
	}


	list = PyList_New(result->numRows);

	/* convert all the user data into a py obj*/
	for (i = 0; i < result->numRows; i++)
	{
		users = result->data[i];
		obj = Py_BuildValue("issi", users->id, users->user_id, users->stream_name, users->num_read);
		freeUserResults(users);

		PyList_SetItem(list, i, obj);
	}

	/* create the result structure */
	obj = Py_BuildValue("iiO", result->numRows, result->numFields, list);

	free(result->data);
	free(result);
	return obj;
}

/* destroys the result struct */
static PyObject* genResult_posts(SQL_result* result, int numRead)
{
	int i;
	SQL_post_result* post;
	PyObject* obj;
	PyObject* list;


	if (result == NULL)
	{
		Py_RETURN_NONE;
	}

	list = PyList_New(result->numRows);

	/* convert all the user data into a py obj*/
	for (i = 0; i < result->numRows; i++)
	{
		post = result->data[i];
		if ( i <= numRead)
		{
			obj = Py_BuildValue("issssi", post->id, post->stream_name, post->user_id, post->date, post->text, 1);
		}
		else
		{
			obj = Py_BuildValue("issssi", post->id, post->stream_name, post->user_id, post->date, post->text, 0);
		}

		freePostResults(post);
		PyList_SetItem(list, i, obj);
	}

	/* create the result structure */
	obj = Py_BuildValue("iiO", result->numRows, result->numFields, list);

	free(result->data);
	free(result);
	return obj;
}




