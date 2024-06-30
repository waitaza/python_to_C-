#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <time.h>
#include <string.h>

// 日期解析函数，支持多种格式
static PyObject* parse_date(PyObject* self, PyObject* args) {
    const char* date_str;
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));

    if (!PyArg_ParseTuple(args, "s", &date_str)) {
        return NULL;
    }

    if (strptime(date_str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        if (strptime(date_str, "%Y-%m-%d", &tm) == NULL) {
            if (strptime(date_str, "%d/%m/%Y", &tm) == NULL) {
                PyErr_SetString(PyExc_ValueError, "Error parsing date");
                return NULL;
            }
        }
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

// 计算时间区间（返回天、小时、分钟、秒）
static PyObject* calculate_interval(PyObject* self, PyObject* args) {
    const char* date_str1;
    const char* date_str2;
    struct tm tm1, tm2;
    time_t t1, t2;
    double diff;
    int days, hours, minutes, seconds;

    memset(&tm1, 0, sizeof(struct tm));
    memset(&tm2, 0, sizeof(struct tm));

    if (!PyArg_ParseTuple(args, "ss", &date_str1, &date_str2)) {
        return NULL;
    }

    if (strptime(date_str1, "%Y-%m-%d %H:%M:%S", &tm1) == NULL) {
        if (strptime(date_str1, "%Y-%m-%d", &tm1) == NULL) {
            PyErr_SetString(PyExc_ValueError, "Error parsing first date");
            return NULL;
        }
    }

    if (strptime(date_str2, "%Y-%m-%d %H:%M:%S", &tm2) == NULL) {
        if (strptime(date_str2, "%Y-%m-%d", &tm2) == NULL) {
            PyErr_SetString(PyExc_ValueError, "Error parsing second date");
            return NULL;
        }
    }

    t1 = mktime(&tm1);
    t2 = mktime(&tm2);
    diff = difftime(t2, t1);

    days = diff / (60 * 60 * 24);
    diff -= days * 60 * 60 * 24;
    hours = diff / (60 * 60);
    diff -= hours * 60 * 60;
    minutes = diff / 60;
    seconds = diff - minutes * 60;

    return Py_BuildValue("iiii", days, hours, minutes, seconds);
}

// 时间戳转换为日期时间
static PyObject* timestamp_to_date(PyObject* self, PyObject* args) {
    time_t timestamp;
    struct tm *tm_info;

    if (!PyArg_ParseTuple(args, "l", &timestamp)) {
        return NULL;
    }

    tm_info = localtime(&timestamp);

    return Py_BuildValue("iiiiiiiii",
                         tm_info->tm_year + 1900,
                         tm_info->tm_mon + 1,
                         tm_info->tm_mday,
                         tm_info->tm_hour,
                         tm_info->tm_min,
                         tm_info->tm_sec,
                         tm_info->tm_wday,
                         tm_info->tm_yday,
                         tm_info->tm_isdst);
}

// 日期时间转换为时间戳
static PyObject* date_to_timestamp(PyObject* self, PyObject* args) {
    int year, month, day, hour, minute, second;
    struct tm tm;

    if (!PyArg_ParseTuple(args, "iiiiii", &year, &month, &day, &hour, &minute, &second)) {
        return NULL;
    }

    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    tm.tm_isdst = -1;

    return Py_BuildValue("l", mktime(&tm));
}

// 计算两个日期之间的天数差异
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

    if (strptime(date_str1, "%Y-%m-%d %H:%M:%S", &tm1) == NULL) {
        if (strptime(date_str1, "%Y-%m-%d", &tm1) == NULL) {
            PyErr_SetString(PyExc_ValueError, "Error parsing first date");
            return NULL;
        }
    }

    if (strptime(date_str2, "%Y-%m-%d %H:%M:%S", &tm2) == NULL) {
        if (strptime(date_str2, "%Y-%m-%d", &tm2) == NULL) {
            PyErr_SetString(PyExc_ValueError, "Error parsing second date");
            return NULL;
        }
    }

    t1 = mktime(&tm1);
    t2 = mktime(&tm2);
    diff = difftime(t2, t1);

    return Py_BuildValue("i", diff / (60 * 60 * 24));
}

// 获取当前时间
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

// 日期增加天数
static PyObject* add_days(PyObject* self, PyObject* args) {
    const char* date_str;
    int days;
    struct tm tm;

    memset(&tm, 0, sizeof(struct tm));

    if (!PyArg_ParseTuple(args, "si", &date_str, &days)) {
        return NULL;
    }

    if (strptime(date_str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        if (strptime(date_str, "%Y-%m-%d", &tm) == NULL) {
            PyErr_SetString(PyExc_ValueError, "Error parsing date");
            return NULL;
        }
    }

    tm.tm_mday += days;

    if (mktime(&tm) == -1) {
        PyErr_SetString(PyExc_ValueError, "Error adjusting date");
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

// 时区转换
static PyObject* convert_timezone(PyObject* self, PyObject* args) {
    const char* date_str;
    int hours_offset;
    struct tm tm;

    memset(&tm, 0, sizeof(struct tm));

    if (!PyArg_ParseTuple(args, "si", &date_str, &hours_offset)) {
        return NULL;
    }

    if (strptime(date_str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        if (strptime(date_str, "%Y-%m-%d", &tm) == NULL) {
            PyErr_SetString(PyExc_ValueError, "Error parsing date");
            return NULL;
        }
    }

    tm.tm_hour += hours_offset;

    if (mktime(&tm) == -1) {
        PyErr_SetString(PyExc_ValueError, "Error adjusting time");
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

// 生成日期范围
static PyObject* generate_date_range(PyObject* self, PyObject* args) {
    const char* start_date_str;
    const char* end_date_str;
    struct tm start_tm, end_tm;
    time_t start_time, end_time;
    double diff;
    int days;
    PyObject* date_list;
    char date_buffer[11];  // YYYY-MM-DD

    memset(&start_tm, 0, sizeof(struct tm));
    memset(&end_tm, 0, sizeof(struct tm));

    if (!PyArg_ParseTuple(args, "ss", &start_date_str, &end_date_str)) {
        return NULL;
    }

    if (strptime(start_date_str, "%Y-%m-%d", &start_tm) == NULL) {
        PyErr_SetString(PyExc_ValueError, "Error parsing start date");
        return NULL;
    }

    if (strptime(end_date_str, "%Y-%m-%d", &end_tm) == NULL) {
        PyErr_SetString(PyExc_ValueError, "Error parsing end date");
        return NULL;
    }

    start_time = mktime(&start_tm);
    end_time = mktime(&end_tm);
    diff = difftime(end_time, start_time);

    if (diff < 0) {
        PyErr_SetString(PyExc_ValueError, "End date must be after start date");
        return NULL;
    }

    days = diff / (60 * 60 * 24);
    date_list = PyList_New(0);

    for (int i = 0; i <= days; i++) {
        start_tm.tm_mday += 1;
        mktime(&start_tm);
        strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", &start_tm);
        PyList_Append(date_list, PyUnicode_FromString(date_buffer));
    }

    return date_list;
}

// 格式化日期字符串
static PyObject* format_date(PyObject* self, PyObject* args) {
    int year, month, day, hour, minute, second;
    const char* format;
    struct tm tm;
    char buffer[256];

    if (!PyArg_ParseTuple(args, "iiiiiis", &year, &month, &day, &hour, &minute, &second, &format)) {
        return NULL;
    }

    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    tm.tm_isdst = -1;

    if (strftime(buffer, sizeof(buffer), format, &tm) == 0) {
        PyErr_SetString(PyExc_ValueError, "Error formatting date");
        return NULL;
    }

    return PyUnicode_FromString(buffer);
}

// 解析ISO 8601日期字符串
static PyObject* parse_iso8601(PyObject* self, PyObject* args) {
    const char* date_str;
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));

    if (!PyArg_ParseTuple(args, "s", &date_str)) {
        return NULL;
    }

    if (strptime(date_str, "%Y-%m-%dT%H:%M:%S", &tm) == NULL) {
        PyErr_SetString(PyExc_ValueError, "Error parsing ISO 8601 date");
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

// 日期加减时间间隔
static PyObject* add_time_interval(PyObject* self, PyObject* args) {
    const char* date_str;
    int years, months, days, hours, minutes, seconds;
    struct tm tm;

    if (!PyArg_ParseTuple(args, "siiiiii", &date_str, &years, &months, &days, &hours, &minutes, &seconds)) {
        return NULL;
    }

    memset(&tm, 0, sizeof(struct tm));

    if (strptime(date_str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        if (strptime(date_str, "%Y-%m-%d", &tm) == NULL) {
            PyErr_SetString(PyExc_ValueError, "Error parsing date");
            return NULL;
        }
    }

    tm.tm_year += years;
    tm.tm_mon += months;
    tm.tm_mday += days;
    tm.tm_hour += hours;
    tm.tm_min += minutes;
    tm.tm_sec += seconds;

    if (mktime(&tm) == -1) {
        PyErr_SetString(PyExc_ValueError, "Error adjusting date");
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

// 方法定义
static PyMethodDef DateUtilMethods[] = {
    {"parse_date", parse_date, METH_VARARGS, "Parse a date string"},
    {"calculate_interval", calculate_interval, METH_VARARGS, "Calculate the interval between two dates"},
    {"timestamp_to_date", timestamp_to_date, METH_VARARGS, "Convert a timestamp to a date"},
    {"date_to_timestamp", date_to_timestamp, METH_VARARGS, "Convert a date to a timestamp"},
    {"calculate_diff", calculate_diff, METH_VARARGS, "Calculate the difference in days between two dates"},
    {"get_current_time", get_current_time, METH_VARARGS, "Get the current time"},
    {"add_days", add_days, METH_VARARGS, "Add days to a date"},
    {"convert_timezone", convert_timezone, METH_VARARGS, "Convert a date to another timezone"},
    {"generate_date_range", generate_date_range, METH_VARARGS, "Generate a range of dates between two dates"},
    {"format_date", format_date, METH_VARARGS, "Format a date into a string"},
    {"parse_iso8601", parse_iso8601, METH_VARARGS, "Parse an ISO 8601 date string"},
    {"add_time_interval", add_time_interval, METH_VARARGS, "Add a time interval to a date"},
    {NULL, NULL, 0, NULL}
};

// 模块定义
static struct PyModuleDef dateutilmodule = {
    PyModuleDef_HEAD_INIT,
    "dateutil",
    NULL,
    -1,
    DateUtilMethods
};

// 模块初始化
PyMODINIT_FUNC PyInit_dateutil(void) {
    return PyModule_Create(&dateutilmodule);
}
