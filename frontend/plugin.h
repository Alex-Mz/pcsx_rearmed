#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define PLUGIN_DL_BASE 0xfbad0000

enum builtint_plugins_e {
	PLUGIN_GPU,
	PLUGIN_SPU,
	PLUGIN_CDR,
	PLUGIN_PAD,
};

void *plugin_link(enum builtint_plugins_e id, const char *sym);