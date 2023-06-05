#include "../../Common/Manager.h"
#include "../../Common/Antena.h"
#include "../../Common/Amplifier.h"
#include "../../Common/Modem.h"
#include <vector>
#include "../../Common/Component.h"
#include "../../Common/IDD.h"
#include <iostream>
#include <stdio.h>
#include <iostream>

int main()
{

    vector<controlerType> controllers = { ANTENA,AMPLIFIER,MODEM };
    Antena antena(TRANSMITTER);
    Amplifier amplifier(TRANSMITTER);
    Modem modem(TRANSMITTER);
    Manager manager(controllers, TRANSMITTER);
    while (1) {
        continue;
    }
}
