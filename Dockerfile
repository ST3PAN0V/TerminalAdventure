FROM gcc:11.3 as build

# Установка зависимостей нужных
RUN apt update && apt install -y cmake

# Копирование исходников
COPY ./sources /app/sources
COPY ./CMakeLists.txt /app/

# Сборка
RUN mkdir /app/build && cd /app/build && cmake .. && cmake --build .

# точка вохода
ENTRYPOINT ["./app/build/TerminalAdventure"] 