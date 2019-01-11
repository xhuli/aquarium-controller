#include <stdio.h>
#include "DosingTask.h"

int main() {
    DosingTask dosingTask = DosingTask();
    dosingTask.dayOfWeek = 5;
    dosingTask.startHour = 23;
    dosingTask.startMinute = 58;
    dosingTask.doseMiliLiters = 254;

    printf("%d\tdayOfWeek", sizeof(dosingTask.dayOfWeek));
    printf("%d\tstartHour", sizeof(dosingTask.startHour));
    printf("%d\tstartMinute", sizeof(dosingTask.startMinute));
    printf("%d\tdoseMiliLiters", sizeof(dosingTask.doseMiliLiters));

    printf("%d\tdosingTask", sizeof(dosingTask));

    return 0;
}