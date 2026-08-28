/* ... existing enums ... */

/* Set the entire active channel mask at once.
   By default, all channels are enabled (~0u). */
void remom_log_set_channel_mask(unsigned int mask);

/* Enable or disable specific channels at runtime. */
void remom_log_enable_channel(RemomLogChannel channel);
void remom_log_disable_channel(RemomLogChannel channel);

/* ... existing write, pump, and shutdown functions ... */