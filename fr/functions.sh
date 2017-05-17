#!/usr/bin/env bash

jv_pg_aj_echo_plant_status()
{
  plant_issue_display=""

  case "$1" in
    humidity_ok) ;;
    humidity_error) plant_issue_display+="erreur de récupération de l'humidité, ";;
    humidity_wet) plant_issue_display+="elle manque d'eau, ";;
    humidity_dry) plant_issue_display+="elle a trop d'eau, ";;
    *) jv_error "Erreur: Clef de traduction '$1' non reconnue. Merci de contacter le concepteur de ce plugin.";;
  esac

  case "$2" in
    temp_ok) ;;
    temp_error) plant_issue_display+="erreur de récupération de la température, ";;
    temp_cold) plant_issue_display+="elle a froid, ";;
    temp_hot) plant_issue_display+="elle a chaud, ";;
    *) jv_error "Erreur: Clef de traduction '$2' non reconnue. Merci de contacter le concepteur de ce plugin.";;
  esac

  case "$3" in
    luminosity_ok) ;;
    luminosity_error) plant_issue_display+="erreur de récupération de la température, ";;
    luminosity_low) plant_issue_display+="elle n'est pas assez au soleil, ";;
    luminosity_high) plant_issue_display+="elle est trop au soleil, ";;
    *) jv_error "Erreur: Clef de traduction '$3' non reconnue. Merci de contacter le concepteur de ce plugin.";;
  esac

  if [[ $plant_issue_display == "" ]]; then
    echo "La plante va bien."
  else
    echo "La plante ne va pas très bien: ${plant_issue_display}..."
  fi
}

jv_pg_aj_lang()
{
  case "$1" in
    temperature_is) echo "La temperature est de $2 degrés";;
    invalid_temperature) echo "Impossible de récupérer la température...";;
    humidity_is) echo "L'humidité de la plante est de $2 pourcent";;
    invalid_humidity) echo "Impossible de récupérer l'humidité de la plante...";;
    luminosity_is) echo "La luminosité est de $2 lux";;
    invalid_luminosity) echo "Impossible de récupérer la luminosité...";;
    plant_is_ok) jv_pg_aj_echo_plant_status $2 $3 $4;;
    invalid_plant_ok_request) echo "Impossible de savoir si la plante va bien...";;
    *) jv_error "Erreur: Clef de traduction '$1' non reconnue. Merci de contacter le concepteur de ce plugin.";;
  esac
}
