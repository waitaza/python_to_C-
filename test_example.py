import dateutil

# Parse a date
date = dateutil.parse_date("2024-06-30")
print(date)

# Calculate the interval between two dates
interval = dateutil.calculate_interval("2024-06-30", "2024-07-01")
print(interval)

# Convert timestamp to date
date = dateutil.timestamp_to_date(1593561600)
print(date)

# Convert date to timestamp
timestamp = dateutil.date_to_timestamp(2024, 6, 30, 0, 0, 0)
print(timestamp)

# Calculate the difference in days between two dates
days_diff = dateutil.calculate_diff("2024-06-30", "2024-07-01")
print(days_diff)

# Get the current time
current_time = dateutil.get_current_time()
print(current_time)

# Add days to a date
new_date = dateutil.add_days("2024-06-30", 5)
print(new_date)

# Convert a date to another timezone
new_timezone_date = dateutil.convert_timezone("2024-06-30 00:00:00", 5)
print(new_timezone_date)

# Generate a range of dates
date_range = dateutil.generate_date_range("2024-06-01", "2024-06-05")
print(date_range)

# Format a date
formatted_date = dateutil.format_date(2024, 6, 30, 0, 0, 0, "%Y-%m-%d %H:%M:%S")
print(formatted_date)

# Parse ISO 8601 date string
iso_date = dateutil.parse_iso8601("2024-06-30T00:00:00")
print(iso_date)

# Add time interval to a date
new_date = dateutil.add_time_interval("2024-06-30 00:00:00", 1, 0, 0, 0, 0, 0)
print(new_date)
