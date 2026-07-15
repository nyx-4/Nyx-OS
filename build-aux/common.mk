# This build system handles cross-compilation, where the compilation happens on
# the machine BUILD, and produces executables that will run on machine HOST,
# whose output (if any) would run on machine TARGET. By default, it will assume
# BUILD == HOST == TARGET, unless overridden.

# Determine the platform per uname.
UNAME_OS:=$(shell uname -s | tr '[:upper:]' '[:lower:]' | sed 's,^linux$$,linux-gnu,g')
UNAME_MACHINE:=$(shell uname -m)

# Detect the platform that the software is being built on.
BUILD?=$(UNAME_MACHINE)-$(UNAME_OS)

# Determine the platform the software will run on.
HOST?=$(BUILD)
HOST_MACHINE:=$(shell expr x$(HOST) : 'x\([^-]*\).*')

# Determine the platform the software will target.
TARGET?=$(HOST)

# Determine the per-arch directory. For i*86-elf, use i386.
HOSTARCH:=$(if $(filter i%86,$(HOST_MACHINE)),i386,$(HOST_MACHINE))

# Determine the prefix for host tools.
ifneq ($(BUILD),$(HOST))
  HOST_TOOL_PREFIX?=$(HOST)-
endif

# Utility function to determine whether a variable is unset or its value is a
# default provided by make. Those defaults must be overridden below, but any
# other user choices must be respected.
is_unset_or_default = $(filter undefined default,$(origin $(1)))

# Determine the names of the cross-tools that target the host platform.
ifneq ($(BUILD),$(HOST))
  ifneq ($(call is_unset_or_default,CC),)
    CC:=$(HOST_TOOL_PREFIX)gcc
  endif
  ifneq ($(call is_unset_or_default,AR),)
    AR:=$(HOST_TOOL_PREFIX)ar
  endif
  ifneq ($(call is_unset_or_default,AS),)
    AS:=nasm
  endif
endif

# Inform the toolchain about the location of the sysroot if overridden.
ifdef SYSROOT
  CC:=$(CC) --sysroot="$(SYSROOT)"
endif

# Determine the system directory structure.
PREFIX?=/usr
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=/boot
LIBDIR?=$(EXEC_PREFIX)/lib
INCLUDEDIR?=$(PREFIX)/include

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
ifneq ($(filter %-elf,$(HOST)),)
  CC+=-isystem=$(INCLUDEDIR)
endif