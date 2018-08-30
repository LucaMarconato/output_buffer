#ifndef OUTPUT_BUFFER_H
#define OUTPUT_BUFFER_H

#include <string>
#include <fstream>

class Output_buffer {
    std::ofstream out;
    char * buffer;
    unsigned long buffer_size;
    unsigned long max_chunk_size;
    unsigned long current_index;
    bool need_save;
    std::string filename;
public:
    Output_buffer(std::string filename, unsigned long buffer_size, unsigned long max_chunk_size);
    void add_chunk(void * data, unsigned long size);
    ~Output_buffer();
    void write_to_file();
};

#endif //OUTPUT_BUFFER_H
