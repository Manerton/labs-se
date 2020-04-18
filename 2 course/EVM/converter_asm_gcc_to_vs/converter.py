# -*- coding: utf-8 -*-

#Скрипт для Notepad++, конвертирующий код C++ GCC ASM в C++ Visual Studio ASM

text = editor.getText()

import re
# поменяем объявление вставки и R"(
text = re.sub('(asm __volatile__ \()\r\n(.*)', '__asm {', text)
# уберем закрывающую ) от R"(, а также список входных, выходных операндов, и меняем закрывающую вставку круглую скобку на фигурную
text = re.sub('(\)")\r\n(.*)\r\n(.*)\r\n(.*)', '}', text)
# поменяем стиль комментариев с # на //
text = re.sub('#(?!include)', '//', text)
def getInner():
		perem = re.search('(?<=%\[)(.*)(?=\])', text)
		return perem.group(0)

def getOuter():
		perem = re.search('(%\[(.*)\])', text)
		return perem

while getOuter():
		inner = getInner()
		text = re.sub('(%\[(.*)\])', inner, text, 1)

editor.setText(text)
