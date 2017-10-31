SceCtrlData mp_keys;

unsigned int mp_GetKey(void) {
//	sceCtrlReadBufferPositive(&mp_keys, 1);
	sceCtrlPeekBufferPositive(&mp_keys, 1);

	return mp_keys.Buttons;
}
