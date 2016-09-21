// Copyright (c) 2013 GitHub, Inc.
// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef ATOM_BROWSER_NET_ATOM_URL_REQUEST_H_
#define ATOM_BROWSER_NET_ATOM_URL_REQUEST_H_

#include "base/memory/ref_counted.h"
#include "net/url_request/url_request.h"


namespace net {
class IOBuffer;
class IOBufferWithSize;
class DrainableIOBuffer;
}

namespace atom {

class AtomBrowserContext;

namespace api {
  class URLRequest;
}

class AtomURLRequest : public base::RefCountedThreadSafe<AtomURLRequest>,
                       public net::URLRequest::Delegate {
public:
  static scoped_refptr<AtomURLRequest> create(
    AtomBrowserContext* browser_context,
    const std::string& url,
    base::WeakPtr<api::URLRequest> delegate);

  void set_method(const std::string& method);

  void Write();
  void End();
  void Abort();
  void SetHeader();
  void GetHeader();
  void RemoveHeader();

  scoped_refptr<net::HttpResponseHeaders> GetResponseHeaders();

protected:
  // Overrides of net::URLRequest::Delegate
  virtual void OnResponseStarted(net::URLRequest* request) override;
  virtual void OnReadCompleted(net::URLRequest* request, int bytes_read) override;

private:
  friend class base::RefCountedThreadSafe<AtomURLRequest>;
  void StartOnIOThread();

  void ReadResponse();
  bool CopyAndPostBuffer(int bytes_read);

  void InformDelegateResponseStarted();
  void InformDelegateResponseData(scoped_refptr<net::IOBufferWithSize> data);
  void InformDelegateResponseCompleted();

  AtomURLRequest(base::WeakPtr<api::URLRequest> delegate);
  virtual ~AtomURLRequest();

  base::WeakPtr<api::URLRequest> delegate_;
  std::unique_ptr<net::URLRequest> url_request_;
  scoped_refptr<net::IOBuffer> buffer_;
  scoped_refptr<net::HttpResponseHeaders> response_headers_;

   DISALLOW_COPY_AND_ASSIGN(AtomURLRequest);
 };

}  // namespace atom

#endif  // ATOM_BROWSER_NET_ATOM_URL_REQUEST_H_