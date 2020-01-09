inline uint32_t asm_set_1_and_3_flags(uint32_t a)
{
    const uint32_t mask = 0b101; // -- 1 и 3 бит -- //
    __asm {
        mov eax, a    	// поместим переменную a в регистр eax
        mov ebx, mask   // а маску в ebx
        or eax, ebx     // выполним операцию ИЛИ
        mov a, eax    	// результат поместим в переменную a
      }
    return a;
}
