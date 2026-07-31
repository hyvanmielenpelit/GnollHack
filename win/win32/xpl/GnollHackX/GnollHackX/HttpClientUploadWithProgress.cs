using System;
using System.IO;
using System.Net;
using System.Net.Http;
using System.Threading;
using System.Threading.Tasks;

namespace GnollHackX
{
    public class HttpClientUploadWithProgress : IDisposable
    {
        private readonly string _uploadUrl;
        private readonly HttpContent _content;
        private CancellationTokenSource _cancellationTokenSource;
        private HttpClient _httpClient;

        public delegate void ProgressChangedHandler(
            long? totalFileSize, long totalBytesUploaded, double? progressPercentage);

        public event ProgressChangedHandler ProgressChanged;

        public HttpClientUploadWithProgress(string uploadUrl, HttpContent content,
            CancellationTokenSource cancellationTokenSource)
        {
            _uploadUrl = uploadUrl;
            _content = content;
            _cancellationTokenSource = cancellationTokenSource;
        }

        public HttpResponseMessage Response { get; private set; }

        public async Task StartUpload()
        {
            _httpClient = new HttpClient { Timeout = TimeSpan.FromMinutes(5) };

            /* Serialize content to a MemoryStream so we know the total size */
            using (var serializedStream = new MemoryStream())
            {
                await _content.CopyToAsync(serializedStream);
                var totalSize = serializedStream.Length;
                serializedStream.Position = 0;

                var progressContent = new ProgressStreamContent(
                    serializedStream, totalSize, _cancellationTokenSource,
                    (total, uploaded, pct) =>
                    {
                        ProgressChanged?.Invoke(total, uploaded, pct);
                    });

                /* Copy headers from original content */
                foreach (var header in _content.Headers)
                    progressContent.Headers.TryAddWithoutValidation(header.Key, header.Value);

                Response = await _httpClient.PostAsync(
                    _uploadUrl, progressContent, _cancellationTokenSource.Token);
            }
        }

        public void Dispose()
        {
            _httpClient?.Dispose();
        }
    }

    internal class ProgressStreamContent : HttpContent
    {
        private readonly Stream _stream;
        private readonly long _totalSize;
        private readonly CancellationTokenSource _cts;
        private readonly Action<long?, long, double?> _onProgress;

        public ProgressStreamContent(Stream stream, long totalSize,
            CancellationTokenSource cts, Action<long?, long, double?> onProgress)
        {
            _stream = stream;
            _totalSize = totalSize;
            _cts = cts;
            _onProgress = onProgress;
        }

        protected override async Task SerializeToStreamAsync(Stream stream, TransportContext context)
        {
            var buffer = new byte[8192];
            long totalBytesWritten = 0;
            long readCount = 0;
            int bytesRead;

            while ((bytesRead = await _stream.ReadAsync(buffer, 0, buffer.Length)) > 0)
            {
                await stream.WriteAsync(buffer, 0, bytesRead);
                totalBytesWritten += bytesRead;
                readCount++;

                if (readCount % 10 == 0 || bytesRead < buffer.Length)
                {
                    var pct = _totalSize == 0 ? 0 :
                        Math.Round((double)totalBytesWritten / _totalSize * 100, 2);
                    _onProgress?.Invoke(_totalSize, totalBytesWritten, pct);
                }

                if (_cts != null)
                    _cts.Token.ThrowIfCancellationRequested();
            }
        }

        protected override bool TryComputeLength(out long length)
        {
            length = _totalSize;
            return true;
        }
    }
}
