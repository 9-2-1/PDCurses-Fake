// begin processing stdin
void _richia_init();
// write message
void _richia_write(const char *format, ...);
// create new request and wait reply
char *_richia_ask_and_wait(const char *format, ...);
// flush stdout
void _richia_flush();
// free reply message
void _richia_freemsg();
// stop processing stdin
void _richia_close();
