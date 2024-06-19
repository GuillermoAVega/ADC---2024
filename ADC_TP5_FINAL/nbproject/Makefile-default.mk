#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=${DISTDIR}/proyectobase4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=${DISTDIR}/proyectobase4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=source/config.c source/isr_UART2.c source/isr_timer46.c source/main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/source/config.o ${OBJECTDIR}/source/isr_UART2.o ${OBJECTDIR}/source/isr_timer46.o ${OBJECTDIR}/source/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/source/config.o.d ${OBJECTDIR}/source/isr_UART2.o.d ${OBJECTDIR}/source/isr_timer46.o.d ${OBJECTDIR}/source/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/source/config.o ${OBJECTDIR}/source/isr_UART2.o ${OBJECTDIR}/source/isr_timer46.o ${OBJECTDIR}/source/main.o

# Source Files
SOURCEFILES=source/config.c source/isr_UART2.c source/isr_timer46.c source/main.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/proyectobase4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ256GP710
MP_LINKER_FILE_OPTION=,--script=p33FJ256GP710.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/source/config.o: source/config.c  .generated_files/flags/default/244660f35e336d28a8a6f6f7dc7eec0dd7c2cded .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/config.o.d 
	@${RM} ${OBJECTDIR}/source/config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  source/config.c  -o ${OBJECTDIR}/source/config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/source/config.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../../../../../../opt/microchip/xc16/v1.20/support/dsPIC33F/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/source/isr_UART2.o: source/isr_UART2.c  .generated_files/flags/default/73990554b8bbd49a6595fc128965bf98d4f5e0b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/isr_UART2.o.d 
	@${RM} ${OBJECTDIR}/source/isr_UART2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  source/isr_UART2.c  -o ${OBJECTDIR}/source/isr_UART2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/source/isr_UART2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../../../../../../opt/microchip/xc16/v1.20/support/dsPIC33F/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/source/isr_timer46.o: source/isr_timer46.c  .generated_files/flags/default/4bdafa4a3a9508f531079450d12526b7b99896c3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/isr_timer46.o.d 
	@${RM} ${OBJECTDIR}/source/isr_timer46.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  source/isr_timer46.c  -o ${OBJECTDIR}/source/isr_timer46.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/source/isr_timer46.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../../../../../../opt/microchip/xc16/v1.20/support/dsPIC33F/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/source/main.o: source/main.c  .generated_files/flags/default/2a1ee3050da2ec32e95b661309c2dea61776d183 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/main.o.d 
	@${RM} ${OBJECTDIR}/source/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  source/main.c  -o ${OBJECTDIR}/source/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/source/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../../../../../../opt/microchip/xc16/v1.20/support/dsPIC33F/h" -msmart-io=1 -Wall -msfr-warn=off   
	
else
${OBJECTDIR}/source/config.o: source/config.c  .generated_files/flags/default/77a8453f3747929891f1e60e545d5b1f55b9f413 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/config.o.d 
	@${RM} ${OBJECTDIR}/source/config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  source/config.c  -o ${OBJECTDIR}/source/config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/source/config.o.d"        -g -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../../../../../../opt/microchip/xc16/v1.20/support/dsPIC33F/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/source/isr_UART2.o: source/isr_UART2.c  .generated_files/flags/default/b182cd6c7aedbe604ae13852d8db1d16a2b49e0e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/isr_UART2.o.d 
	@${RM} ${OBJECTDIR}/source/isr_UART2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  source/isr_UART2.c  -o ${OBJECTDIR}/source/isr_UART2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/source/isr_UART2.o.d"        -g -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../../../../../../opt/microchip/xc16/v1.20/support/dsPIC33F/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/source/isr_timer46.o: source/isr_timer46.c  .generated_files/flags/default/a3f2df34b59c93818efa5a197c7c55dbc8dd929b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/isr_timer46.o.d 
	@${RM} ${OBJECTDIR}/source/isr_timer46.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  source/isr_timer46.c  -o ${OBJECTDIR}/source/isr_timer46.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/source/isr_timer46.o.d"        -g -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../../../../../../opt/microchip/xc16/v1.20/support/dsPIC33F/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/source/main.o: source/main.c  .generated_files/flags/default/6b069cdfcc9dac49959b8a760f8dc3e21211db57 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/main.o.d 
	@${RM} ${OBJECTDIR}/source/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  source/main.c  -o ${OBJECTDIR}/source/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/source/main.o.d"        -g -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../../../../../../opt/microchip/xc16/v1.20/support/dsPIC33F/h" -msmart-io=1 -Wall -msfr-warn=off   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/proyectobase4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/proyectobase4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="../../../../../../../../opt/microchip/xc16/v1.20/support/dsPIC33F",--no-force-link,--smart-io,-Map="${DISTDIR}/proyectobase4.X.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	
else
${DISTDIR}/proyectobase4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/proyectobase4.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="../../../../../../../../opt/microchip/xc16/v1.20/support/dsPIC33F",--no-force-link,--smart-io,-Map="${DISTDIR}/proyectobase4.X.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/proyectobase4.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a    
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
