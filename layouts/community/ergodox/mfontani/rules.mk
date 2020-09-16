# Only lists overrides; see keyboards/ergodox_ez/rules.mk for defaults
LTO_ENABLE = yes
CONSOLE_ENABLE = yes
# TAP_DANCE_ENABLE = yes
OPT_DEFS += -DUSER_PRINT
LEADER_ENABLE = yes
EXTRAFLAGS += -flto
WPM_ENABLE = yes
