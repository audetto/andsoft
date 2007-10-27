## Process this file with automake to produce Makefile.in

INCLUDES = \
	@GTK_CFLAGS@

bin_PROGRAMS = chat

chat_SOURCES = \
	main.c \
	main_win.c main_win.h \
        client_win.c client_win.h \
        function.c function.h \
        data.c data.h \
        launch.c launch.h \
        about.c about.h

chat_LDADD = @GTK_LIBS@

