//===--- UUID.cpp - UUID generation ---------------------------------------===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2016 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
//
// This is an interface over the standard OSF uuid library that gives UUIDs
// sane value semantics and operators.
//
//===----------------------------------------------------------------------===//
#if defined(_MSC_VER)
// Avoid defining macro max(), min() which conflict with std::max(), std::min()
#define NOMINMAX
#include <Rpc.h>
#else
#include <uuid/uuid.h>
#endif
#include "swift/Basic/UUID.h"
#include <stdio.h>

using namespace swift;

swift::UUID::UUID(FromRandom_t) {
#if defined(_MSC_VER)
  UuidCreate((_GUID *)&Value);
#else
  uuid_generate_random(Value);
#endif
}

swift::UUID::UUID(FromTime_t) {
#if defined(_MSC_VER)
  UuidCreate((_GUID *)&Value);
#else
  uuid_generate_time(Value);
#endif
}

swift::UUID::UUID() {
#if defined(_MSC_VER)
  UuidCreateNil((_GUID *)&Value);
#else
  uuid_clear(Value);
#endif
}

Optional<swift::UUID> swift::UUID::fromString(const char *s) {
  swift::UUID result;
#if defined(_MSC_VER)
  if (UuidFromString(RPC_CSTR(s), (_GUID *)&result) != RPC_S_OK)
#else
  if (uuid_parse(s, result.Value))
#endif
    return None;
  return result;
}

void swift::UUID::toString(llvm::SmallVectorImpl<char> &out) const {
  out.resize(UUID::StringBufferSize);
#if defined(_MSC_VER)
  RPC_CSTR  UUIDString;
  UuidToString((const GUID *)Value, &UUIDString);
  memcpy(out.data(), UUIDString, StringBufferSize);
  RpcStringFree(&UUIDString);
#else
  uuid_unparse_upper(Value, out.data());
#endif
  // Pop off the null terminator.
  assert(out.back() == '\0' && "did not null-terminate?!");
  out.pop_back();
}

int swift::UUID::compare(swift::UUID y) const {
#if defined(_MSC_VER)
  RPC_STATUS status;
  return UuidCompare((GUID *)Value, (GUID *)&y, &status);
#else
  return uuid_compare(Value, y.Value);
#endif
}

llvm::raw_ostream &swift::operator<<(llvm::raw_ostream &os, swift::UUID uuid) {
  llvm::SmallString<UUID::StringBufferSize> buf;
  uuid.toString(buf);
  os << buf;
  return os;
}
