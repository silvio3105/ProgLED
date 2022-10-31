
# RGB LED Library

This is frameworkless library for controlling programmable RGB LEDs. Library does not depend on framework (eg., Arduino or STM HAL). External handlers for sending(clocking) LED's data have to be provided during object construction. Library is written in C++ and it supports multiple LED lines. One LED line can control up to 65536 LEDs(**theoretically, not HW tested**).
RGB and GRB color formats are supported. Library is made for controlling `IN-PI55TATPRPGPB` RGB LED.

Library documentation is available at `.docs/html/index.html` Example applications are available at `examples` folder. All examples are made with STM HAL framework and tested with `IN-PI55TATPRPGPB` LEDs on custom hardware for 3D Clock project.


# License

Copyright (c) 2022, silvio3105 (www.github.com/silvio3105)

Access and use of this Project and its contents are granted free of charge to any Person.
The Person is allowed to copy, modify and use The Project and its contents only for non-commercial use.
Commercial use of this Project and its contents is prohibited.
Modifying this License and/or sublicensing is prohibited.

THE PROJECT AND ITS CONTENT ARE PROVIDED "AS IS" WITH ALL FAULTS AND WITHOUT EXPRESSED OR IMPLIED WARRANTY.
THE AUTHOR KEEPS ALL RIGHTS TO CHANGE OR REMOVE THE CONTENTS OF THIS PROJECT WITHOUT PREVIOUS NOTICE.
THE AUTHOR IS NOT RESPONSIBLE FOR DAMAGE OF ANY KIND OR LIABILITY CAUSED BY USING THE CONTENTS OF THIS PROJECT.

This License shall be included in all functional textual files.