/*  This file is part of the Tufão project
    Copyright (C) 2011 Vinícius dos Santos Oliveira <vini.ipsmaker@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any
    later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HTTPSERVERRESPONSE_H
#define HTTPSERVERRESPONSE_H

#include <QObject>
#include "tufao_global.h"

class QIODevice;
template <class Key, class T> class QMap;

namespace Tufao {

namespace Priv {

struct HttpServerResponse;

} // namespace Priv

/*!
  \brief The Tufao::HttpServerResponse is used to respond to a
  Tufao::HttpServerRequest.

  \sa Tufao::HttpServer
  */
class TUFAOSHARED_EXPORT HttpServerResponse : public QObject
{
    Q_OBJECT
public:
    enum StatusCode
    {
        // 1xx Informational
        CONTINUE                        = 100,
        SWITCHING_PROTOCOLS             = 101,
        PROCESSING                      = 102,
        CHECKPOINT                      = 103,
        // 2xx Successful
        OK                              = 200,
        CREATED                         = 201,
        ACCEPTED                        = 202,
        NON_AUTHORITATIVE_INFORMATION   = 203,
        NO_CONTENT                      = 204,
        RESET_CONTENT                   = 205,
        PARTIAL_CONTENT                 = 206,
        MULTI_STATUS                    = 207,
        ALREADY_REPORTED                = 208,
        IM_USED                         = 226,
        // 3xx Redirection
        MULTIPLE_CHOICES                = 300,
        MOVED_PERMANENTLY               = 301,
        FOUND                           = 302,
        SEE_OTHER                       = 303,
        NOT_MODIFIED                    = 304,
        USE_PROXY                       = 305,
        SWITCH_PROXY                    = 306,
        TEMPORARY_REDIRECT              = 307,
        RESUME_INCOMPLETE               = 308,
        // 4xx Client Error
        BAD_REQUEST                     = 400,
        UNAUTHORIZED                    = 401,
        PAYMENT_REQUIRED                = 402,
        FORBIDDEN                       = 403,
        NOT_FOUND                       = 404,
        METHOD_NOT_ALLOWED              = 405,
        NOT_ACCEPTABLE                  = 406,
        PROXY_AUTHENTICATION_REQUIRED   = 407,
        REQUEST_TIMEOUT                 = 408,
        CONFLICT                        = 409,
        GONE                            = 410,
        LENGTH_REQUIRED                 = 411,
        PRECONDITION_FAILED             = 412,
        REQUEST_ENTITY_TOO_LARGE        = 413,
        REQUEST_URI_TOO_LONG            = 414,
        UNSUPPORTED_MEDIA_TYPE          = 415,
        REQUESTED_RANGE_NOT_SATISFIABLE = 416,
        EXPECTATION_FAILED              = 417,
        I_AM_A_TEAPOT                   = 418,
        UNPROCESSABLE_ENTITY            = 422,
        LOCKED                          = 423,
        FAILED_DEPENDENCY               = 424,
        UNORDERED_COLLECTION            = 425,
        UPGRADE_REQUIRED                = 426,
        PRECONDITION_REQUIRED           = 428,
        TOO_MANY_REQUESTS               = 429,
        REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
        NO_RESPONSE                     = 444,
        RETRY_WITH                      = 449,
        CLIENT_CLOSED_REQUEST           = 499,
        // 5xx Internal Server Error
        INTERNAL_SERVER_ERROR           = 500,
        NOT_IMPLEMENTED                 = 501,
        BAD_GATEWAY                     = 502,
        SERVICE_UNAVAILABLE             = 503,
        GATEWAY_TIMEOUT                 = 504,
        HTTP_VERSION_NOT_SUPPORTED      = 505,
        VARIANT_ALSO_NEGOTIATES         = 506,
        INSUFFICIENT_STORAGE            = 507,
        LOOP_DETECTED                   = 508,
        BANDWIDTH_LIMIT_EXCEEDED        = 509,
        NOT_EXTENDED                    = 510
    };

    enum Option
    {
        HTTP_1_0           = 1,
        HTTP_1_1           = 1 << 1,
        CLOSE_CONNECTION   = 1 << 2,
        USE_CHUNKED_ENTITY = 1 << 3
    };
    Q_DECLARE_FLAGS(Options, Option)

    /*!
      Constructs a Tufao::HttpServerResponse object.

      \p parent is passed to the QObject constructor.

      \p options controls the response behaviour.

      \param device The socket used by Tufao::HttpServerResponse to write a HTTP
      response message. If you pass NULL, the object will do nothing.
      */
    explicit HttpServerResponse(QIODevice *device,
                                Options options,
                                QObject *parent = 0);

    /*!
      Destroys the object.
      */
    ~HttpServerResponse();

    /*!
      Returns the options passed to the object constructor.
      */
    Options options() const;

    /*!
      Sets a single header value for implicit headers. If this header already
      exists in the to-be-sent headers, it's value will be replaced.

      \param name The header key.
      \param value The header value.
      */
    void setHeader(const QByteArray &name, const QByteArray &value);

    /*!
      Reads out a header that's already been queued but not sent to the client.

      \note The name is case insensitive. This can only be called before headers
      get implicitly flushed (when you start to send the entity body).
      */
    QByteArray header(const QByteArray &name);

    /*!
      Removes a header that's queued for implicit sending.
      */
    void removeHeader(const QByteArray &name);

signals:
    /*!
      This signal is emitted when all bytes from the HTTP response message are
      written in the device/socket.

      Call Tufao::HttpServerResponse::end will cause this signal to be emitted.
      */
    void finished();

public slots:
    /*!
      Sends a <em>HTTP/1.1 100 Continue</em> message to the client, indicating
      that the request body should be sent.
      */
    bool writeContinue();

    /*!
      Sends a response header to the request.

      Call this function after the first chunk of entity body data was already
      sent (calling Tufao::HttpServerResponse::write ) will have no effect.

      \param statusCode The status code is a 3-digit HTTP status code.
      \param reasonPhrase A human-readable reasonPhrase.
      \param headers The response headers.
      */
    bool writeHead(int statusCode, const QByteArray &reasonPhrase,
                   const QMap<QByteArray, QByteArray> &headers);

    /*!
      This is an overloaded function.

      \sa
      Tufao::HttpServerResponse::writeHead(int, const QByteArray&, const QMap<QByteArray,QByteArray>&)
      */
    bool writeHead(int statusCode, const QMap<QByteArray, QByteArray> &headers);

    /*!
      This is an overloaded function.

      \sa
      Tufao::HttpServerResponse::writeHead(int, const QByteArray&, const QMap<QByteArray,QByteArray>&)
      */
    bool writeHead(int statusCode);

    /*!
      This sends a chunk of the response body. This method may be called
      multiple times to provide successive parts of the body.

      \note This is the raw HTTP body and has nothing to do with higher-level
      multi-part body encodings that may be used.

      The first time Tufao::HttpServerResponse::write is called, it will send
      the buffered header information and the first body chunk to the client.
      The second time Tufao::HttpServerResponse::write is called, Tufao assumes
      you're going to be streaming data, and sends that separately. That is, the
      response is buffered up to the first chunk of body.

      If you call this function with a empty byte array, it will do nothing.

      \warning Note that HTTP/1.0 user agents don't support chunked entities. So
      you need to send the full message at once. This member function will do
      nothing if you are servicing a response to a HTTP/1.0 user agent. Use
      Tufao::HttpServerResponse::end in place.
      */
    bool write(const QByteArray &chunk);

    /*!
      This method adds HTTP trailing headers (a header but at the end of the
      message) to the response.

      \warning Trailers will only be emitted if chunked encoding is used for the
      response; if it is not (e.g., if the request was sent by a HTTP/1.0 user
      agent), they will be silently discarded.

      \note A server MUST NOT use the trailer for any header fields unless at
      least one of the following is true:
        - the request included a TE header field that indicates “trailers” is
          acceptable in the transfer-coding of the response;
        - the server is the origin server (your server is not a proxy or a
          tunnel) for the response, the trailer fields consist entirely of
          optional metadata, and the recipient could use the message (in a
          manner acceptable to the origin server) without receiving this
          metadata. In other words, the origin server is willing to accept the
          possibility that the trailer fields might be silently discarded along
          the path to the client.
      */
    bool addTrailers(const QMap<QByteArray, QByteArray> &headers);

    /*!
      This method adds one HTTP trailing header (a header but at the end of the
      message) to the response.

      \sa Tufao::HttpServerResponse::addTrailers
      */
    bool addTrailer(const QByteArray &headerName,
                    const QByteArray &headerValue);

    /*!
      This method signals to the server that all of the response headers and
      body has been sent; that server should consider this message complete. The
      method, response.end(), MUST be called on each response.

      \param chunk If specified, it is equivalent to calling
      Tufao::HttpServerResponse::write followed by
      Tufao::HttpServerResponse::end
      */
    bool end(const QByteArray &chunk = QByteArray());

private:
    Priv::HttpServerResponse *priv;
};

} // namespace Tufao

#endif // HTTPSERVERRESPONSE_H
