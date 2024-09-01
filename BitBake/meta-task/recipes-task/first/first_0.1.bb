DESCRIPTION = "Compile the Hmm.c file from the Heap Memory Manager project"
PR = "r1"

# Define the source directory
S = "${WORKDIR}/../Heap Memory Manager"

OUTPUT_DIR = "${WORKDIR}/../output"

do_compile() {
    # Compile Hmm.c into Hmm.o
    ${CC} ${CFLAGS} ${LDFLAGS} ${S}/Hmm.c -o ${S}/Hmm.o
}

do_install() {
    # Create the custom destination directory and install the compiled object file
    install -d ${OUTPUT_DIR}
    install -m 0755 ${S}/Hmm.o ${OUTPUT_DIR}/
}
