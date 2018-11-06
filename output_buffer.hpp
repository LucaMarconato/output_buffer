#ifndef OUTPUT_BUFFER_H
#define OUTPUT_BUFFER_H

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>

class Output_buffer {   
    std::ofstream out;
    char * buffer;
    unsigned long buffer_size;
    unsigned long max_chunk_size;
    unsigned long current_index;
    bool need_save;
    std::string filename;
    
public:

    bool verbose = false;
    Output_buffer(std::string filename, unsigned long buffer_size, unsigned long max_chunk_size)
    {
        out.open(filename, std::ios::out | std::ios::binary);
        if(!out.is_open()) {
            std::cerr << "error in Output_buffer(): unable to open " << filename << "\n";
            exit(1);
        }
        
        if(max_chunk_size > floor(buffer_size/2)) {
            std::cerr << "error: max_chunk_size = " << max_chunk_size << ", buffer_size = " << buffer_size << "\n";
            exit(1);
        }

        this->buffer_size = buffer_size;
        this->max_chunk_size = max_chunk_size;
        this->buffer = new char [buffer_size];
        this->current_index = 0;
        this->need_save = false;
        this->filename = filename;
    }
    
    void add_chunk(void * data, unsigned long size)
    {
        if(this->current_index + size > this->buffer_size) {
            std::cerr << "error: this->current_index = " << this->current_index << ", size = " << size << ", this->buffer_size = " << this->buffer_size << "\n";
            exit(1);
        }
        if(size == 0) {
            std::cerr << "warning: add_chunk, size == 0\n";
        }
        memcpy(this->buffer+this->current_index, data, size);
        this->current_index += size;
        this->need_save = true;
        if(this->current_index >= this->buffer_size - this->max_chunk_size) {
            this->write_to_file();
            this->current_index = 0;        
        }
    }

    void add_chunk(std::string & s)
    {
        this->add_chunk((char *)s.c_str(), s.size());
    }

    void write_to_file()
    {
        if(!out.is_open()) {
            std::cerr << "error in write_to_file(): " << this->filename << "file is not open\n";
            exit(1);        
        }
        if(this->current_index > 0) {
            out.write(this->buffer, this->current_index);
            if(verbose) {
                if(current_index >= 1024*1024) {
                    std::cout << "written " << std::setprecision(2) << (this->current_index)/(1024*1024) << "MB to " << this->filename << "\n";
                } else if(current_index >= 1024) {
                    std::cout << "written " << std::setprecision(2) << (this->current_index)/1024 << "KB to " << this->filename << "\n";
                } else {
                    std::cout << "written " << this->current_index << "B to " << this->filename << "\n";
                }   
            }
            this->need_save = false;
        }
    }
    
    ~Output_buffer()
    {
        if(need_save) {
            write_to_file();
        }
        delete [] buffer;
        out.close();
    }
};

#endif //OUTPUT_BUFFER_H
