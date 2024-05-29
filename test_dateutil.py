import dateutil_c

def test_parse_date():
    date_str = "2023-05-29"
    result = dateutil_c.parse_date(date_str)
    expected = (2023, 5, 29, 0, 0, 0, 0, 148, -1)
    assert result == expected, f"Expected {expected}, but got {result}"

def test_calculate_diff():
    date_str1 = "2023-05-29"
    date_str2 = "2023-06-29"
    result = dateutil_c.calculate_diff(date_str1, date_str2)
    expected = 31.0
    assert result == expected, f"Expected {expected}, but got {result}"

def test_get_current_time():
    result = dateutil_c.get_current_time()
    assert len(result) == 9, "Expected 9 elements in the result"

def test_add_days():
    date_str = "2023-05-29"
    days = 10
    result = dateutil_c.add_days(date_str, days)
    expected = (2023, 6, 8, 0, 0, 0, 3, 158, -1)
    assert result == expected, f"Expected {expected}, but got {result}"

def test_convert_timezone():
    date_str = "2023-05-29T12:00:00"
    hours_offset = -5
    result = dateutil_c.convert_timezone(date_str, hours_offset)
    expected = (2023, 5, 29, 7, 0, 0, 1, 148, -1)
    assert result == expected, f"Expected {expected}, but got {result}"

if __name__ == "__main__":
    test_parse_date()
    test_calculate_diff()
    test_get_current_time()
    test_add_days()
    test_convert_timezone()
    print("All tests passed.")
