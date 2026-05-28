#include "remom_log.h"
#include <string.h>

int main(int argc, char** argv) {
    int i;
    
    /* Initialize the file handles and rotation */
    remom_log_init();

    /* For Release mode, you might want to mute high-traffic channels by default */
    remom_log_disable_channel(LOG_CHAN_INPUT);
    remom_log_disable_channel(LOG_CHAN_RENDER);

    /* Parse command line arguments to selectively re-enable them */
    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--debug-input") == 0) {
            remom_log_enable_channel(LOG_CHAN_INPUT);
            remom_log_write(LOG_INFO, LOG_CHAN_CORE, "Input logging explicitly enabled via CLI.");
        }
        else if (strcmp(argv[i], "--debug-all") == 0) {
            /* Turn everything back on */
            remom_log_set_channel_mask(~0u); 
            remom_log_write(LOG_INFO, LOG_CHAN_CORE, "All logging channels enabled via CLI.");
        }
    }

    /* Rest of the engine startup... */
    
    return 0;
}