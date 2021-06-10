Build guide:
1. Папку runtime с заголовками от ANTLR Runtime положить в папку с проектом (включая подпапку src в папке runtime).
2. Надо скомпилить библиотеку: проекты antlr4_shared (.dll .dll.a) и antlr_static (.a) (которые лежат например в antlr4-cpp-runtime-4.9.2-source.zip) через CMake. Вызывать CMake через консоль, например: cmake --build *АБСОЛЮТНЫЙ ПУТЬ* --target antlr4_static, т.к через Qt Creator выдает какую-то ошибку.
3. Положить lib и dll куда-нибудь в PATH (например в папку с MinGW, по папкам bin и lib), чтобы либа подключилась через -l, иначе можно прописать абсолютные пути к библиотеке с ANTLR Runtime через соответствующие команды в CMake.
4. В папку с проектом положить ANTLR.jar
5. Запустить generate.cmd/generate.sh
6. Скомпилить проект через CMake.

В launch.json - настройка дебага ANTLR грамматики в VS Code.
.g4 файлы - ANTLR грамматика.