#ifndef MSGPACK_ADAPTORS_H
#define MSGPACK_ADAPTORS_H

#include "color.h"
#include "gamevector.h"
#include <msgpack.hpp>

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {

  template <>
  struct convert<game::Color> {
    msgpack::object const& operator()(msgpack::object const& o, game::Color& color) const
    {
      if (o.type != msgpack::type::ARRAY)
        throw msgpack::type_error();
      if (o.via.array.size != 4)
        throw msgpack::type_error();

      color = game::Color{
        o.via.array.ptr[0].as<std::uint8_t>(),
        o.via.array.ptr[1].as<std::uint8_t>(),
        o.via.array.ptr[2].as<std::uint8_t>(),
        o.via.array.ptr[3].as<std::uint8_t>()
      };

      return o;
    }
  };

  template <>
  struct pack<game::Color> {
    template <typename Stream>
    msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, game::Color const& color) const
    {
      o.pack_array(4);
      o.pack_uint8(color.r);
      o.pack_uint8(color.g);
      o.pack_uint8(color.b);
      o.pack_uint8(color.a);

      return o;
    }
  };

  template <typename T>
  struct convert<game::Vector<T>> {
    msgpack::object const& operator()(msgpack::object const& o, game::Vector<T>& v) const
    {
      if (o.type != msgpack::type::ARRAY)
        throw msgpack::type_error();
      if (o.via.array.size != 2)
        throw msgpack::type_error();

      v = game::Vector<T>{
        o.via.array.ptr[0].as<T>(),
        o.via.array.ptr[1].as<T>()
      };

      return o;
    }
  };

  template <typename T>
  struct pack<game::Vector<T>> {
    template <typename Stream>
    msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, game::Vector<T> const& v) const
    {
      o.pack_array(2);
      o.pack(v.x);
      o.pack(v.y);

      return o;
    }
  };

} // namespace adaptor
} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack

#endif // MSGPACK_ADAPTORS_H