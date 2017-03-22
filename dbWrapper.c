
#include <Python.h>
#include "dbAccess.h"


/* to be called by python */
static PyObject* db_getUserStreams(PyObject *self, PyObject *args);

static PyObject* db_getStreamPosts(PyObject *self, PyObject *args);


/* internal */
static PyObject* genResult_streams(SQL_result* result);

static PyObject* genResult_posts(SQL_result* result);


void initdbWrapper();



/* the list of the functions */
static PyMethodDef module_method_getStreams[] =
{
	{ "getUsersStreams", (PyCFunction)db_getStreamPosts, METH_VARARGS, NULL },
	{ NULL, NULL, 0, NULL }
};

static PyMethodDef module_method_getPosts[] =
{
	{ "getStreamPosts", (PyCFunction)db_getUserStreams, METH_VARARGS, NULL },
	{ NULL, NULL, 0, NULL }
};



void initdbWrapper()
{
	Py_InitModule3("getUsersStreams", module_method_getStreams, "");
	Py_InitModule3("getStreamPosts", module_method_getPosts, "");
}





static PyObject* db_getUserStreams(PyObject *self, PyObject *args)
{
	MYSQL* mysql;
	SQL_result* result;
	char* userID;

	userID = NULL;

	if (PyArg_ParseTuple(args, "s", userID) == 0)
	{
		return NULL;
	}

	mysql = initSQL();

	/* get the data */
	result = getUserStreams(mysql, userID);

	mysql_close(mysql);


	return genResult_streams(result);
}

static PyObject* db_getStreamPosts(PyObject *self, PyObject *args)
{
	MYSQL* mysql;
	SQL_result* result;
	char* stream;

	stream = NULL;

	if (PyArg_ParseTuple(args, "s", stream) == 0)
	{
		return NULL;
	}

	mysql = initSQL();

	/* get the data */
	result = getStreamPosts(mysql, stream);

	mysql_close(mysql);


	return genResult_posts(result);
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
static PyObject* genResult_posts(SQL_result* result)
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
		obj = Py_BuildValue("issss", post->id, post->stream_name, post->user_id, post->date, post->text);

		freePostResults(post);
		PyList_SetItem(list, i, obj);
	}

	/* create the result structure */
	obj = Py_BuildValue("iiO", result->numRows, result->numFields, list);

	free(result->data);
	free(result);
	return obj;
}




