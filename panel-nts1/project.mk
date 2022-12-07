# #############################################################################
# Project Customization
# #############################################################################

# This file is included from the platform specific makefile, which is copied from the SDK.
# Put platform specific Makefile customisations here. Put the platform independent
# customisations in $(SRCDIR)/project.mk

PLATFORM_UDEFS = -DDSP_CORTEX_M4 -DDSP_KORG_LOGUE -DPLATFORM_KORG_NTS1

# Include the project specific definitions.
include ../src/project.mk

# Override the SDK path definitions
PLATFORMDIR = $(SDKDIR)/platform/nutekt-digital
PROJECTDIR = .
TOOLSDIR = $(SDKDIR)/tools
EXTDIR = $(SDKDIR)/platform/ext
CMSISDIR = $(EXTDIR)/CMSIS/CMSIS

# Ensure the correct project name and version in manifest.json
ifeq ($(strip $(DISPLAY_NAME)),)
    DISPLAY_NAME=$(PROJECT)
endif

all: manifest.json

manifest.json: $(SRCDIR)/project.mk
	@cat manifest.json | sed "s/\"name\".*,/\"name\" \: \"$(DISPLAY_NAME)\",/" | sed "s/\"version\".*,/\"version\" \: \"$(VERSION)\",/" > __manifest_temp__
	@mv __manifest_temp__ manifest.json
	@echo "manifest.json was updated"
