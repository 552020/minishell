#include <stdio.h>
#include <unistd.h>

int main() {
    int slot;

    slot = ttyslot();
    
    if (slot == -1) {
        printf("ttyslot() failed.\n");
        return 1;
    }

    printf("The slot index is: %d\n", slot);
    return 0;
}

