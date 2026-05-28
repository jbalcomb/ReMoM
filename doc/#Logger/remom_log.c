/* ... existing includes and defines ... */

static char s_RingBuffer[REMOM_LOG_BUFFER_SIZE];
static size_t s_Head = 0;
static size_t s_Tail = 0;

static FILE* s_LogFile = NULL;
static int s_FileDisabled = 0;

/* NEW: The bitmask. ~0u sets all bits to 1, enabling everything by default. */
static unsigned int s_ActiveChannelsMask = ~0u;

/* ... existing name arrays ... */

/* NEW: Mask control implementations */
void remom_log_set_channel_mask(unsigned int mask) {
    s_ActiveChannelsMask = mask;
}

void remom_log_enable_channel(RemomLogChannel channel) {
    s_ActiveChannelsMask |= (1u << channel);
}

void remom_log_disable_channel(RemomLogChannel channel) {
    s_ActiveChannelsMask &= ~(1u << channel);
}

/* ... init function remains the same ... */

void remom_log_write(RemomLogLevel level, RemomLogChannel channel, const char* fmt, ...) {
    char tempBuf[REMOM_LOG_TEMP_SIZE];
    int charsWritten;
    size_t length;
    size_t i;
    size_t capacity;
    va_list args;

    /* NEW: Early exit if this channel is currently muted.
       This prevents unnecessary string formatting and CPU usage. */
    if ((s_ActiveChannelsMask & (1u << channel)) == 0) {
        return;
    }

    /* 1. Format the prefix with both Level and Channel */
    int prefixLen = sprintf(tempBuf, "[%s][%s] ", s_LevelNames[level], s_ChannelNames[channel]);
    if (prefixLen < 0) return;

    /* ... the rest of the string formatting and buffer writing remains exactly the same ... */