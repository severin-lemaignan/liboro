#########################
# Autoconf requirements #
#########################
AC_INIT(liboro,0.3.2,slemaign@laas.fr)
AC_PREREQ(2.59)

#######################
# Package information #
#######################
AC_LANG(C++)

######################
# Check for programs #
######################
AC_PROG_CXX

AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)
AC_SUBST(PACKAGE_VERSION)
AC_SUBST(PATH_YARP)

AC_ARG_WITH(yarp,
	[AS_HELP_STRING([--with-yarp],
        [specify path to Yarp library])],
        PATH_YARP=$withval)


LIBS="$LIBS -lYARP_OS"

CPPFLAGS="$CPPFLAGS -I$PATH_YARP/include"
LDFLAGS="$LDFLAGS -L$PATH_YARP/lib"

#AC_CHECK_LIB(YARP_OS, ) # <a ajouter

AC_CONFIG_FILES(src/Makefile Makefile)
AC_OUTPUT
echo "Now, type 'make & make install' to compile and install liboro to the provided prefix, 'make test' to compile and run a oro server benchmark."
