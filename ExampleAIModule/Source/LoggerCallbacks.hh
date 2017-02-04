#pragma once

#ifndef LOGGERCALLBACKS_HH_
#define LOGGERCALLBACKS_HH_

#include <functional>
#include <iostream>
#include <memory>
#include <fstream>

namespace LoggerCallbacks
{
  typedef std::function<void(const std::string&)> ProcessorType;

  class WriteToStream
  {
  public:
    WriteToStream(std::ostream& stream);
    ~WriteToStream();

    void  operator()(const std::string& str);

  private:
    std::ostream* _stream;
  };

  class WriteToFile
  {
  public:
    WriteToFile(const std::string& pathName,
                std::ofstream::openmode mode = std::ofstream::out);
    ~WriteToFile();

    void  operator()(const std::string& str);

  private:
    std::shared_ptr<WriteToStream>  _writter;
    std::shared_ptr<std::ofstream>  _stream;
  };


} // namespace LoggerCallbacks

#endif // LOGGERCALLBACKS_HH_