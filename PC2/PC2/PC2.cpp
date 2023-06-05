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

    vector<controlerType> controllers = { ANTENA,MODEM };
    Antena antena(RECEIVER);
    Modem modem(RECEIVER);
    Manager manager(controllers, RECEIVER);
    while (1) {
        continue;
    }
}
