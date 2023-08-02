using System;
using System.Collections.Generic;
using System.IO;
using System.Net.Http;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace GnollHackX
{
    public class HttpClientDownloadWithProgress : IDisposable
    {
        private readonly string _downloadUrl;
        private readonly string _destinationFilePath;
        private CancellationTokenSource _cancellationTokenSource;
        private HttpClient _httpClient;

        public delegate void ProgressChangedHandler(long? totalFileSize, long totalBytesDownloaded, double? progressPercentage);

        public event ProgressChangedHandler ProgressChanged;

        public HttpClientDownloadWithProgress(string downloadUrl, string destinationFilePath, CancellationTokenSource cancellationTokenSource)
        {
            _downloadUrl = downloadUrl;
            _destinationFilePath = destinationFilePath;
            _cancellationTokenSource = cancellationTokenSource;
        }

        public async Task StartDownload()
        {
            _httpClient = new HttpClient { Timeout = TimeSpan.FromDays(1) };

            using (var response = await _httpClient.GetAsync(_downloadUrl, HttpCompletionOption.ResponseHeadersRead, _cancellationTokenSource.Token))
            {
                await DownloadFileFromHttpResponseMessage(response);
            }
        }

        private async Task DownloadFileFromHttpResponseMessage(HttpResponseMessage response)
        {
            response.EnsureSuccessStatusCode();

            var totalBytes = response.Content.Headers.ContentLength;

            using (var contentStream = await response.Content.ReadAsStreamAsync())
            {
                await ProcessContentStream(totalBytes, contentStream);
            }
        }

        private async Task ProcessContentStream(long? totalDownloadSize, Stream contentStream)
        {
            var totalBytesRead = 0L;
            var readCount = 0L;
            var buffer = new byte[8192];
            var isMoreToRead = true;

            using (var fileStream = new FileStream(_destinationFilePath, FileMode.Create, FileAccess.Write, FileShare.None, 8192, true))
            {
                do
                {
                    var bytesRead = await contentStream.ReadAsync(buffer, 0, buffer.Length);
                    if (bytesRead == 0)
                    {
                        isMoreToRead = false;
                        TriggerProgressChanged(totalDownloadSize, totalBytesRead);
                        continue;
                    }

                    await fileStream.WriteAsync(buffer, 0, bytesRead);

                    totalBytesRead += bytesRead;
                    readCount += 1;

                    if (readCount % 100 == 0)
                        TriggerProgressChanged(totalDownloadSize, totalBytesRead);

                    if(_cancellationTokenSource.Token.IsCancellationRequested)
                    {
                        _cancellationTokenSource.Token.ThrowIfCancellationRequested();
                    }
                }
                while (isMoreToRead);
            }
        }

        private void TriggerProgressChanged(long? totalDownloadSize, long totalBytesRead)
        {
            if (ProgressChanged == null)
                return;

            double? progressPercentage = null;
            if (totalDownloadSize.HasValue)
                progressPercentage = Math.Round((double)totalBytesRead / totalDownloadSize.Value * 100, 2);

            ProgressChanged(totalDownloadSize, totalBytesRead, progressPercentage);

        }

        public void Dispose()
        {
            _httpClient?.Dispose();
        }
    }
}
