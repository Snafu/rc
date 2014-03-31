# RC Car XMC4500

RC car following an object via Wiimote IR sensor.



## IR Custom Pinout


                     +---------------+
                +----+---------------+----+
                |                         |
                |                         |
                +-------------------------+
                  |  |     . . .     |  |
                  |  9     . . .     |  1
                 10                  2


              +-----+-----+-----+-----+------+
            9 | GND | VCC | SCL | CLK | GND  |  1
              +-----+-----+-----+-----+------+
           10 | NC  | GND | #CE | SDA | #RST |  2
              +-----+-----+-----+-----+------+


## Links

* [Bluetooth module](http://docs-europe.electrocomponents.com/webdocs/125a/0900766b8125a3e6.pdf)
* [Instructables IR cam](http://www.instructables.com/id/Wii-Remote-IR-Camera-Hack/?lang=de)
* [Kotaku](http://translate.google.com/translate?u=http%3A%2F%2Fwww.kako.com%2Fneta%2F2008-009%2F2008-009.html&hl=en&ie=UTF-8&sl=ja&tl=en)
* [Working with Pixart cam](http://procrastineering.blogspot.co.at/2008/09/working-with-pixart-camera-directly.html)
