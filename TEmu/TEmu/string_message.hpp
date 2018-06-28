
#ifndef STRING_MESSAGE_HPP
#define STRING_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

class string_message
{
public:
  enum { header_length = 4 };
  enum { max_body_length = 30000};

  string_message()
    : body_length_(0)
  {
  }

  void from_string(const std::string& str)
  {
	  body_length(str.size());
	  memcpy(body(), str.c_str(), body_length());
	  encode_header();
  }

  char* to_char()
  {
	  data_[length()] = 0;
	  return data_;
  }

  std::string to_string()
  {
	  data_[length()] = 0;
	  return std::string(data_);
  }

  const char* data() const
  {
    return data_;
  }

  char* data()
  {
    return data_;
  }

  size_t length() const
  {
    return header_length + body_length_;
  }

  const char* body() const
  {
    return data_ + header_length;
  }

  char* body()
  {
    return data_ + header_length;
  }

  size_t body_length() const
  {
    return body_length_;
  }

  void body_length(size_t new_length)
  {
    body_length_ = new_length;
    if (body_length_ > max_body_length)
      body_length_ = max_body_length;
  }

  bool decode_header()
  {
    using namespace std; // For strncat and atoi.
    char header[header_length + 1] = "";
    strncat(header, data_, header_length);
    body_length_ = atoi(header);
	//body_length_ = *(size_t *)(data_);
	std::cout << "body_length_ : " << body_length_ << std::endl;
    if (body_length_ > max_body_length)
    {
      body_length_ = 0;
      return false;
    }
    return true;
  }

  void encode_header()
  {
    using namespace std; // For sprintf and memcpy.
    char header[header_length + 1] = "";
    sprintf(header, "%4d", static_cast<int>(body_length_));
    memcpy(data_, header, header_length);
	//*(size_t *)(data_) = body_length_;
  }

private:
  char data_[header_length + max_body_length];
  size_t body_length_;
};

#endif // STRING_MESSAGE_HPP
