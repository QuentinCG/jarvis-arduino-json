#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
  Get data from JSON data in Arduino board)
"""
__author__ = 'Quentin Comte-Gaz'
__email__ = "quentin@comte-gaz.com"
__license__ = "MIT License"
__copyright__ = "Copyright Quentin Comte-Gaz (2016)"
__python_version__ = "2.7+ and 3.+"
__version__ = "1.1 (2017/03/25)"
__status__ = "Designed for Jarvis project but not restrictive"

import json, requests

"""Get JSON data from Arduino board and put data to easily usable format
  Keyword arguments:
    json_data_url -- (string) Website/IP to get JSON data

  return: ({"position":"ARDUINO_POSITION_HERE", "sensor_i_name_here":"data_from_sensor_i_here", ...}) List of retrieved data
"""
def getAllData(json_data_url):
  url = requests.get(json_data_url)

  arduino_data = json.loads(url.text)

  result = {}
  if arduino_data.has_key('moduleInfo'):
    if arduino_data['moduleInfo'].has_key('position'):
      result['position'] = str(arduino_data['moduleInfo']['position'])

  if arduino_data.has_key('sensors'):
    for sensor in arduino_data['sensors']:
      if sensor.has_key('name') and sensor.has_key('type') and sensor.has_key('unit') and sensor.has_key('value'):
        result[sensor['type']] = sensor['value']

  return result

"""Get arduino board position
  Keyword arguments:
    json_data_url -- (string) Website/IP to get JSON data

  return: (string) Arduino board position
"""
def getPosition(json_data_url):
  try:
    return getAllData(json_data_url)['position']
  except KeyError:
    return -1

"""Get temperature from Arduino board
  Keyword arguments:
    json_data_url -- (string) Website/IP to get JSON data

  return: (float) Temperature in degres (celcius)
"""
def getTemperature(json_data_url):
  try:
    return getAllData(json_data_url)['temperature']
  except KeyError:
    return -1

"""Get plant humidity from Arduino board
  Keyword arguments:
    json_data_url -- (string) Website/IP to get JSON data

  return: (int) Plant humidity (in %)
"""
def getHumidity(json_data_url):
  try:
    return getAllData(json_data_url)['humidity']
  except KeyError:
    return -1

"""Get luminosity from Arduino board
  Keyword arguments:
    json_data_url -- (string) Website/IP to get JSON data

  return: (float) Luminosity (in lux)
"""
def getLuminosity(json_data_url):
  try:
    return getAllData(json_data_url)['luminosity']
  except KeyError:
    return -1

"""Get luminosity from Arduino board
  Keyword arguments:
    json_data_url -- (string) Website/IP to get JSON data

  return: (bool) Someone is detected or not
"""
def getMotion(json_data_url):
  try:
    return getAllData(json_data_url)['motion']
  except KeyError:
    return -1

"""Check if plant is ok
  Keyword arguments:
    json_data_url -- (string) Website/IP to get JSON data
    min_temp -- (int) Minimum temperature for the plant (in degres celcius)
    max_temp -- (int) Maximum temperature for the plant (in degres celcius)
    min_humidity -- (int) Minimum humidity for the plant (in %)
    max_humidity -- (int) Maximum humidity for the plant (in %)
    min_luminosity -- (int) Minimum luminosity for the plant (in lux)
    max_luminosity -- (int) Maximum luminosity for the plant (in lux)

  return : x, y, z with
           x=-1 if error, 0 if humidity ok, 1 if dry, 2 if wet
           y=-1 if error, 0 if temperature ok, 1 if cold, 2 if too hot
           z=-1 if error, 0 if ok luminosity, 1 if not enough luminosity, 2 if too much luminosity
"""
def isPlantOk(json_data_url, min_temp, max_temp, min_humidity, max_humidity, min_luminosity, max_luminosity):
  is_humidity_ok = -1
  is_temp_ok = -1
  is_luminosity_ok = -1

  try:
    all_data = getAllData(json_data_url)
    temperature = all_data['temperature']
    humidity = all_data['humidity']
    luminosity = all_data['luminosity']

    if temperature < min_temp:
      is_temp_ok = 1
    elif temperature > max_temp:
      is_temp_ok = 2
    else:
      is_temp_ok = 0

    if humidity < min_humidity:
      is_humidity_ok = 1
    elif humidity > max_humidity:
      is_humidity_ok = 2
    else:
      is_humidity_ok = 0

    if luminosity < min_luminosity:
      is_luminosity_ok = 1
    elif luminosity > max_luminosity:
      is_luminosity_ok = 2
    else:
      is_luminosity_ok = 0
  except KeyError:
    pass

  return is_humidity_ok, is_temp_ok, is_luminosity_ok
