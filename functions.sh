#!/usr/bin/env bash

jv_pg_aj_update_stored_motion_detection_datetime()
{
  current_motion=`date +'%Y_%m_%d-%H:%M:%S'`
  echo $current_motion > store/last_motion_detection.txt
}

# Check if same day
# return: 0 if same day, else 1
jv_pg_aj_is_same_day()
{
  local dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
  previous_motion_date=`head -1 $dir/store/last_motion_detection.txt | cut -c1-10`
  current_motion_date=`date +'%Y_%m_%d'`

  if [ "$previous_motion_date" == "$current_motion_date" ]; then
    return 0
  fi

  return 1
}

# $1 (str) Time with "%H:%M:%S" format
# return with echo: (int) Number of seconds since minight of $1
jv_pg_aj_convert_time_to_sec_since_midnight()
{
  temp_time=$1

  time_s=${temp_time##*:}   # Extract the seconds from $1
  time_h=${temp_time%%:*}   # Extract the hours from $1
  time_m=${temp_time%:*}    # Extract the minutes from $1 (step 1)
  time_m=${time_m#*:}   # Extract the minutes from $1 (step 2)

  echo "$((time_h * 3600 + time_m * 60 + time_s))"
}

# Get time between detection
# return with echo: (int) Time (in sec) since last motion detection happened
jv_pg_aj_time_in_sec_since_last_motion_detection()
{
  previous_motion_time=`head -1 store/last_motion_detection.txt | cut -c12-19`
  previous_motion_time_s_sm=`jv_pg_aj_convert_time_to_sec_since_midnight $previous_motion_time`

  current_motion_time=`date +'%H:%M:%S'`
  current_motion_time_s_sm=`jv_pg_aj_convert_time_to_sec_since_midnight $current_motion_time`

  if [ $previous_motion_time_s_sm -gt $current_motion_time_s_sm ]; then # $current_motion_time must be on the next day
    current_motion_time_s_sm=$((current_motion_time_s_sm + 60 * 60 * 24))
  fi

  echo $((current_motion_time_s_sm - previous_motion_time_s_sm))
}

# Say the temperature value in celcius
jv_pg_aj_temperature()
{
  local dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
  local result=`python -c "execfile('$dir/getData/getData.py'); print(getTemperature(\"$var_jv_pg_aj_url\"))"`

  if [ "$result" != "-1" ]; then
    say "$(jv_pg_aj_lang temperature_is $result)"
  else
    say "$(jv_pg_aj_lang invalid_temperature)"
  fi
}

# Say the plant humidity value in percent
jv_pg_aj_humidity()
{
  local dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
  local result=`python -c "execfile('$dir/getData/getData.py'); print(getHumidity(\"$var_jv_pg_aj_url\"))"`

  if [ "$result" != "-1" ]; then
    say "$(jv_pg_aj_lang humidity_is $result)"
  else
    say "$(jv_pg_aj_lang invalid_humidity)"
  fi
}

# Say the luminosity value in lux
jv_pg_aj_luminosity()
{
  local dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
  local result=`python -c "execfile('$dir/getData/getData.py'); print(getLuminosity(\"$var_jv_pg_aj_url\"))"`

  if [ "$result" != "-1" ]; then
    say "$(jv_pg_aj_lang luminosity_is $result)"
  else
    say "$(jv_pg_aj_lang invalid_luminosity)"
  fi
}

# Say if the plant is ok or not with current humidity/luminosity/temperature data
jv_pg_aj_plant_ok()
{
  local dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
  local result=`python -c "execfile('$dir/getData/getData.py'); print(isPlantOk(\"$var_jv_pg_aj_url\", $var_jv_pg_aj_min_temp, $var_jv_pg_aj_max_temp, $var_jv_pg_aj_min_humidity, $var_jv_pg_aj_max_humidity, $var_jv_pg_aj_min_luminosity, $var_jv_pg_aj_max_luminosity))"`

  if [[ "$result" != *"-1"* ]]; then
    result_humidity=`echo $result | cut -d ',' -f 1 | tr -d ",() "`
    result_temp=`echo $result | cut -d ',' -f 2 | tr -d ",() "`
    result_luminosity=`echo $result | cut -d ',' -f 3 | tr -d ",() "`
    #say "humidity=$result_humidity, temp=$result_temp, luminosity=$result_luminosity"

    str_arguments=""

    if [[ "$result_humidy" -eq "0" ]]; then
      str_arguments+="humidity_ok "
      #say "humidity ok"
    elif [[ "$result_humidy" -eq "1" ]]; then
      str_arguments+="humidity_dry "
      #say "humidity dry"
    elif [[ "$result_humidy" -eq "2" ]]; then
      str_arguments+="humidity_wet "
      #say "humidity wet"
    else
      str_arguments+="humidity_error "
      #say "humidity error"
    fi

    if [[ "$result_temp" -eq "0" ]]; then
      str_arguments+="temp_ok "
      #say "temperature ok"
    elif [[ "$result_temp" -eq "1" ]]; then
      str_arguments+="temp_cold "
      #say "temperature cold"
    elif [[ "$result_temp" -eq "2" ]]; then
      str_arguments+="temp_hot "
      #say "temperature hot"
    else
      str_arguments+="temp_error "
      #say "temperature error"
    fi

    if [[ "$result_luminosity" -eq "0" ]]; then
      str_arguments+="luminosity_ok "
      #say "luminosity ok"
    elif [[ "$result_luminosity" -eq "1" ]]; then
      str_arguments+="luminosity_low "
      #say "luminosity low"
    elif [[ "$result_luminosity" -eq "2" ]]; then
      str_arguments+="luminosity_high "
      #say "luminosity high"
    else
      str_arguments+="luminosity_error "
      #say "luminosity error"
    fi

    say "$(jv_pg_aj_lang plant_is_ok $str_arguments)"
  else
    say "$(jv_pg_aj_lang invalid_plant_ok_request)"
  fi
}
