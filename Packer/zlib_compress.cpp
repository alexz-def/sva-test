
#include <iostream>
#include <fstream>
#include "zlib\zutil.h"

bool compress_file(const std::string& input_filename, const std::string& output_filename) 
{
    const size_t CHUNK_SIZE = 16384;

    std::ifstream input_file(input_filename, std::ios::binary);
    std::ofstream output_file(output_filename, std::ios::binary);

    if (!input_file.is_open() || !output_file.is_open())
    {
        std::cerr << "File opening error!" << std::endl;
        return false;
    }

    // Инициализация zlib
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK)
    {
        std::cerr << "Deflate initialization error!" << std::endl;
        return false;
    }

    // Буферы для чтения и записи
    char* in_buffer = new char[CHUNK_SIZE];
    char* out_buffer = new char[CHUNK_SIZE];

    strm.next_out = reinterpret_cast<Bytef*>(out_buffer);
    strm.avail_out = CHUNK_SIZE;

    int flush = Z_NO_FLUSH;
    int ret;

    do
    {
        //input_file.read(input_buffer, CHUNK_SIZE);
        input_file.read(in_buffer, CHUNK_SIZE);

        if (input_file.eof())
            flush = Z_FINISH;

        strm.avail_in = input_file.gcount();
        //strm.next_in = reinterpret_cast<Bytef*>(input_buffer);
        strm.next_in = reinterpret_cast<Bytef*>(in_buffer);

        do
        {
            ret = deflate(&strm, flush);

            if (ret == Z_STREAM_ERROR)
            {
                std::cerr << "Compressing error!" << std::endl;
                deflateEnd(&strm);
                return false;
            }
            if (strm.avail_out == 0)
            {
                output_file.write(out_buffer, CHUNK_SIZE);
                strm.next_out = reinterpret_cast<Bytef*>(out_buffer);
                strm.avail_out = CHUNK_SIZE;
            }

        } while (strm.avail_in > 0);    // || flush == Z_FINISH

    } while (!input_file.eof());

    // Запись оставшихся данных
    if (strm.avail_out < CHUNK_SIZE)
    {
        output_file.write(out_buffer, CHUNK_SIZE - strm.avail_out);
    }

    deflateEnd(&strm);

    input_file.close();
    output_file.close();

    return true;
}
