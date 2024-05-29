#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <time.h>
#include <string.h>

// 日期解析函数
static PyObject* parse_date(PyObject* self, PyObject* args) {
    const char* date_str;
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));

    if (!PyArg_ParseTuple(args, "s", &date_str)) {
        return NULL;
    }

    if (strptime(date_str, "%Y-%m-%d", &tm) == NULL) {
        PyErr_SetString(PyExc_ValueError, "Error parsing date");
        return NULL;
    }

    return Py_BuildValue("iiiiiiiii",
                         tm.tm_year + 1900,
                         tm.tm_mon + 1,
                         tm.tm_mday,
                         tm.tm_hour,
                         tm.tm_min,
                         tm.tm_sec,
                         tm.tm_wday,
                         tm.tm_yday,
                         tm.tm_isdst);
}

// 时间差异计算函数
static PyObject* calculate_diff(PyObject* self, PyObject* args) {
    const char* date_str1;
    const char* date_str2;
    struct tm tm1, tm2;
    time_t t1, t2;
    double diff;

    memset(&tm1, 0, sizeof(struct tm));
    memset(&tm2, 0, sizeof(struct tm));

    if (!PyArg_ParseTuple(args, "ss", &date_str1, &date_str2)) {
        return NULL;
    }

    if (strptime(date_str1, "%Y-%m-%d", &tm1) == NULL) {
        PyErr_SetString(PyExc_ValueError, "Error parsing first date");
        return NULL;
    }

    if (strptime(date_str2, "%Y-%m-%d", &tm2) == NULL) {
        PyErr_SetString(PyExc_ValueError, "Error parsing second date");
        return NULL;
    }

    t1 = mktime(&tm1);
    t2 = mktime(&tm2);
    diff = difftime(t2, t1) / (60 * 60 * 24); // 返回天数差异

    return Py_BuildValue("d", diff);
}

// 获取当前时间函数
static PyObject* get_current_time(PyObject* self, PyObject* args) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    return Py_BuildValue("iiiiiiiii",
                         tm.tm_year + 1900,
                         tm.tm_mon + 1,
                         tm.tm_mday,
                         tm.tm_hour,
                         tm.tm_min,
                         tm.tm_sec,
                         tm.tm_wday,
                         tm.tm_yday,
                         tm.tm_isdst);
}

// 添加时间间隔函数
static PyObject* add_days(PyObject* self, PyObject* args) {
    const char* date_str;
    int days;
    struct tm tm;
    time_t t;

    memset(&tm, 0, sizeof(struct tm));

    if (!PyArg_ParseTuple(args, "si", &date_str, &days)) {
        return NULL;
    }

    if (strptime(date_str, "%Y-%m-%d", &tm) == NULL) {
        PyErr_SetString(PyExc_ValueError, "Error parsing date");
        return NULL;
    }

    t = mktime(&tm);
    t += days * 24 * 60 * 60; // 增加天数

    struct tm* new_tm = localtime(&t);

    return Py_BuildValue("iiiiiiiii",
                         new_tm->tm_year + 1900,
                         new_tm->tm_mon + 1,
                         new_tm->tm_mday,
                         new_tm->tm_hour,
                         new_tm->tm_min,
                         new_tm->tm_sec,
                         new_tm->tm_wday,
                         new_tm->tm_yday,
                         new_tm->tm_isdst);
}

// 转换时区函数
static PyObject* convert_timezone(PyObject* self, PyObject* args) {
    const char* date_str;
    int hours_offset;
    struct tm tm;
    time_t t;

    memset(&tm, 0, sizeof(struct tm));

    if (!PyArg_ParseTuple(args, "si", &date_str, &hours_offset)) {
        return NULL;
    }

    if (strptime(date_str, "%Y-%m-%dT%H:%M:%S", &tm) == NULL) {
        PyErr_SetString(PyExc_ValueError, "Error parsing date");
        return NULL;
    }

    t = mktime(&tm);
    t += hours_offset * 60 * 60; // 调整时区

    struct tm* new_tm = gmtime(&t);

    return Py_BuildValue("iiiiiiiii",
                         new_tm->tm_year + 1900,
                         new_tm->tm_mon + 1,
                         new_tm->tm_mday,
                         new_tm->tm_hour,
                         new_tm->tm_min,
                         new_tm->tm_sec,
                         new_tm->tm_wday,
                         new_tm->tm_yday,
                         new_tm->tm_isdst);
}

// 方法定义
static PyMethodDef DateUtilMethods[] = {
    {"parse_date", parse_date, METH_VARARGS, "Parse a date string"},
    {"calculate_diff", calculate_diff, METH_VARARGS, "Calculate difference between two dates"},
    {"get_current_time", get_current_time, METH_NOARGS, "Get the current date and time"},
    {"add_days", add_days, METH_VARARGS, "Add days to a date"},
    {"convert_timezone", convert_timezone, METH_VARARGS, "Convert date and time to different timezone"},
    {NULL, NULL, 0, NULL}
};

// 模块定义
static struct PyModuleDef dateutilmodule = {
    PyModuleDef_HEAD_INIT,
    "dateutil_c",
    NULL,
    -1,
    DateUtilMethods
};

// 模块初始化函数
PyMODINIT_FUNC PyInit_dateutil_c(void) {
    return PyModule_Create(&dateutilmodule);
}
