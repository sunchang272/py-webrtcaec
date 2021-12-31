/*
 * WebRtcAec.c
 *
 *  Created on: 2021年12月28日13:35:40
 *      Author: Chang Sun
 */

#define SAMPLE_RATE 16000
#define BUFFER_SIZE 160
#include <Python.h>
#include <stdlib.h>
#include "webrtcaec/echo_cancellation.h"

#if PY_MAJOR_VERSION >= 3
#define PY3
#endif

static PyObject *NsError;
const char WebRtcAecDoc[] = "hello.";

static void ns_free(PyObject *nsptr)
{
  void *handle = PyCapsule_GetPointer(nsptr, "WebRtcAecPtr");
  WebRtcAec_Free(handle);
}

static PyObject *ns_create(PyObject *self, PyObject *args)
{
  void *handle = NULL;
  WebRtcAec_Create(&handle);
  PyObject *nsptr;
  nsptr = PyCapsule_New(handle, "WebRtcAecPtr", ns_free);
  return Py_BuildValue("O", nsptr);
}

static PyObject *ns_init(PyObject *self, PyObject *args)
{
  PyObject *nsptr;
  int32_t samplerate = 16000;
  if (!PyArg_ParseTuple(args, "Oi", &nsptr, &samplerate))
  {
    return NULL;
  }
  void *handle = PyCapsule_GetPointer(nsptr, "WebRtcAecPtr");
  int32_t re0 = WebRtcAec_Init(handle, samplerate, samplerate);
  Py_RETURN_NONE;
}

static PyObject *ns_process(PyObject *self, PyObject *args)
{
  PyObject *nsptr;
  Py_buffer far_frame = {NULL, NULL};
  Py_buffer near_frame = {NULL, NULL};
  PyObject *result;
  int32_t msInSndCardBuf = 0;
  int32_t skew = 0;
#ifdef PY3
  if (!PyArg_ParseTuple(args, "Oiiy*y*", &nsptr, &msInSndCardBuf, &skew, &far_frame, &near_frame))
  {
#else
  if (!PyArg_ParseTuple(args, "Oiis*s*", &nsptr, &msInSndCardBuf, &skew, &far_frame, &near_frame))
  {
#endif
    return NULL;
  }
  void *handle = PyCapsule_GetPointer(nsptr, "WebRtcAecPtr");

  short *far_p = (short *)calloc(BUFFER_SIZE, sizeof(short));
  short *near_p = (short *)calloc(BUFFER_SIZE, sizeof(short));
	far_p = (short *)far_frame.buf;
	near_p = (short *)near_frame.buf;

  float out_f[BUFFER_SIZE];
  int32_t re2 = WebRtcAec_BufferFarend(handle, far_p, BUFFER_SIZE);
  int32_t re3 = WebRtcAec_Process(handle, near_p, NULL, out_f, NULL, BUFFER_SIZE, msInSndCardBuf, skew);
  int32_t ec = WebRtcAec_get_error_code(handle);
#ifdef PY3
  result = Py_BuildValue("y#", out_f, BUFFER_SIZE * 2);
#else
  result = Py_BuildValue("s#", out_f, BUFFER_SIZE * 2);
#endif
  PyBuffer_Release(&far_frame);
  PyBuffer_Release(&near_frame);
  return result;
}

static PyMethodDef WebRtcAecMethods[] = {
    {"create", ns_create, METH_NOARGS,
     "Create a ns."},
    {"init", ns_init, METH_VARARGS,
     "Init a ns."},
    {"process", ns_process, METH_VARARGS,
     "Process."},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

#ifdef PY3
static struct PyModuleDef WebRtcAecModule = {
    PyModuleDef_HEAD_INIT,
    "_webrtcaec", /* name of module */
    WebRtcAecDoc, /* module documentation, may be NULL */
    -1,           /* size of per-interpreter state of the module,
                     or -1 if the module keeps state in global variables. */
    WebRtcAecMethods};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit__webrtcaec(void)

#else

#define INITERROR return

void init_webrtcaec(void)

#endif

{
#ifdef PY3
  PyObject *module = PyModule_Create(&WebRtcAecModule);
#else
  PyObject *module = Py_InitModule3("_webrtcaec", WebRtcAecMethods, WebRtcAecDoc);
#endif
  if (module == NULL)
  {
    INITERROR;
  }

  NsError = PyErr_NewException("webrtcaec.Error", NULL, NULL);
  Py_INCREF(NsError);
  PyModule_AddObject(module, "Error", NsError);

#ifdef PY3
  return module;
#endif
}
