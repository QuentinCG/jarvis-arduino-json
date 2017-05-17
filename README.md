## Description
Retrieve data from sensors on Arduino board and use the data with <a target="_blank" href="http://domotiquefacile.fr/jarvis/">Jarvis assistant</a> (JSON protocol is used between the Arduino board and Jarvis assistant)


<img src="https://raw.githubusercontent.com/QuentinCG/jarvis-arduino-json/master/presentation.png" width="800">


## Usage
```
You: Quel est la température dans le salon ?
Jarvis: La température est de 25.37 degrés

You: Quel est la luminosité dans le salon ?
Jarvis: La luminosité est de 20.24 lux

You: Quel est l'humidité de la plante ?
Jarvis: L'humidité de la plante est de 72 pourcent

You: Est-ce que la plante va bien ?
Jarvis: La plante va bien.

Jarvis [asynchrone]: Je te vois !
```


## How to install

1) Install the program and sensors in the Arduino board (<a target="_blank" href="https://github.com/QuentinCG/jarvis-arduino-json/tree/master/arduino">check this page to better understand this part</a>)

2) Add dependency plugin <a target="_blank" href="https://github.com/alexylem/jarvis-api">Jarvis-API</a> to your <a target="_blank" href="http://domotiquefacile.fr/jarvis/">Jarvis assistant</a> (<a target="_blank" href="http://domotiquefacile.fr/jarvis/content/plugins">more info here</a>): ```./jarvis.sh -p https://github.com/alexylem/jarvis-api```

3) Add this plugin to your <a target="_blank" href="http://domotiquefacile.fr/jarvis/">Jarvis assistant</a> (<a target="_blank" href="http://domotiquefacile.fr/jarvis/content/plugins">more info here</a>): ```./jarvis.sh -p https://github.com/QuentinCG/jarvis-arduino-json```

4) Configure the <a target="_blank" href="https://github.com/QuentinCG/jarvis-arduino-json/blob/master/config.sh">configuration file</a> to match your requirements.

5) Enjoy


## Author
[Quentin Comte-Gaz](http://quentin.comte-gaz.com/)


## License

This project is under MIT license. This means you can use it as you want (just don't delete the plugin header).


## Contribute

If you want to add more examples or improve the plugin, just create a pull request with proper commit message and right wrapping.
