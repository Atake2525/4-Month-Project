#include "Audio.h"
#include <cassert>

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"

// チャンクヘッダ
struct ChunkHeader {
	char id[4];   // チャンク毎のID
	int32_t size; // チャンクサイズ
};

// RIFFヘッダチャンク
struct RiffHeader {
	ChunkHeader chunk; // "RIFF"
	char tpye[4];      // "WAVE"
};

// FMTチャンク
struct FormatChunk {
	ChunkHeader chunk; // "fmt"
	WAVEFORMATEX fmt;  // 波形フォーマット
};

const uint32_t Audio::maxSourceVoiceCount = 16;

Audio* Audio::instance = nullptr;


Audio* Audio::GetInstance() {
	if (instance == nullptr) {
		instance = new Audio;
	}
	return instance;
}

void Audio::Finalize() {
	delete instance;
	instance = nullptr;
}

void Audio::Initialize() {
	xAudio2.Reset();
	HRESULT hr = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	hr = xAudio2->CreateMasteringVoice(&masterVoice);

	// mp3読み込みのためのMedia Foundationの初期化
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
}

SoundData Audio::SoundLoadWave(const char* filename, const float volume, const bool isLoop) {
	/// ファイルオープン

	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	// ファイルオープン失敗を検出する
	assert(file.is_open());

	/// .wavデータ読み込み

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	// タイプがWAVEかチェック
	if (strncmp(riff.tpye, "WAVE", 4) != 0) {
		assert(0);
	}

	// Formatチャンクの読み込み
	FormatChunk format = {};
	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	// Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	int time = data.size / format.fmt.nAvgBytesPerSec;

	// Waveファイルを閉じる
	file.close();

	// returnする為の音声データ
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;
	soundData.filename = filename;
	soundData.playTime = time;
	soundData.volume = volume;
	soundData.loop = isLoop;

	return soundData;
}

// 音声再生
void Audio::SoundPlayWave(const SoundData& soundData) {
	HRESULT result;

	// 波形フォーマットをもとにSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// 波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->SetVolume(soundData.volume);
	result = pSourceVoice->Start();
	AudioList list = { pSourceVoice, soundData, frameTime };
	audioList.push_back(list);
	// 指定したsourceVoiceよりも多くpush_backしたらassert
	assert(audioList.size() < maxSourceVoiceCount);
}

void Audio::SoundPlayMp3(const std::wstring& filename) {
	// ソースリーダーの作成
	MFCreateSourceReaderFromURL(filename.c_str(), NULL, &pMFSourceReader);

	// メディアタイプの取得
	// ソースリーダーにPCMで読み込むために
	// MF_MT_MAJOR_TYPEにMFMediaType_Audio, MF_MT_SUBTYPEにMFAudioFormat_PCM
	// を指定してからソースリーダーからメディアタイプを取得
	// 参考 https://qiita.com/ryo_shiraishi6352/items/d4a40521bdb09c838e9d

	IMFMediaType* pMFMediaType{ nullptr };
	MFCreateMediaType(&pMFMediaType);
	pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	pMFSourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pMFMediaType);

	pMFMediaType->Release();
	pMFMediaType = nullptr;
	pMFSourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pMFMediaType);

	WAVEFORMATEX* waveFormat{ nullptr };
	MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &waveFormat, nullptr);

	std::vector<BYTE> mediaData;
	while (true)
	{
		IMFSample* pMFSample{ nullptr };
		DWORD dwStreamFlags{ 0 };
		pMFSourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

		if (dwStreamFlags && MF_SOURCE_READERF_ENDOFSTREAM)
		{
			break;
		}

		IMFMediaBuffer* pMFMediaBuffer{ nullptr };
		pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

		BYTE* pBuffer{ nullptr };
		DWORD cbCurrentLength{ 0 };

		pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

		mediaData.resize(mediaData.size() + cbCurrentLength);
		memcpy(mediaData.data() + mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

		pMFMediaBuffer->Unlock();

	}

	IXAudio2SourceVoice* pSourceVoice{ nullptr };
	xAudio2->CreateSourceVoice(&pSourceVoice, waveFormat);

	HRESULT result;

	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = mediaData.data();
	buffer.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(mediaData.size());
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	result = pSourceVoice->SubmitSourceBuffer(&buffer);
	result = pSourceVoice->SetVolume(0.2f);
	result = pSourceVoice->Start();

}

// 全ての音声停止
void Audio::SoundStopWaveAll() {
	// listに登録されているaudioSourceの全てを音声停止してlistをclearする
	for (AudioList list : audioList)
	{
		list.sourceVoice->Stop();
		list.sourceVoice->DestroyVoice();
	}
	audioList.clear();
}

// 音声停止
void Audio::SoundStopWave(const SoundData& soundData) {
	// listに登録されているaudioSourceの中から指定されたsoundDataのfilenameに一致するもの全てを音声停止して一致するものをlistからremoveする
	uint32_t index = 0;
	uint32_t eraseList[maxSourceVoiceCount] = { 0 };
	uint32_t eraseNum = 0;
	for (AudioList list : audioList)
	{
		if (list.soundData.filename == soundData.filename)
		{
			list.sourceVoice->Stop();
			list.sourceVoice->DestroyVoice();
			eraseList[eraseNum] = index;
			eraseNum++;
		}
		index++;
	}
	for (uint32_t i = 0; i < eraseNum; i++)
	{
		audioList.erase(audioList.begin() + eraseList[i]);
		eraseList[i + 1] -= i + 1;
	}
}

void Audio::Update() {
	// audioListのサイズが0なら早期return
	if (audioList.size() == 0) { 
		frameTime = 0;
		return;
	}
	uint32_t index = 0;
	for (AudioList list : audioList)
	{
		if (frameTime >= list.soundData.playTime * 60 + list.startFrameTime)
		{ // 再生時間ごとに削除 前から再生時間が過ぎたら削除
			list.sourceVoice->Stop();
			list.sourceVoice->DestroyVoice();
			audioList.erase(audioList.begin() + index);
			if (list.soundData.loop)
			{
				if (frameTime >= list.soundData.playTime * 60 + list.startFrameTime)
				{ // ループ再生がonになっている時前から再生時間が過ぎたら再び再生
					SoundPlayWave(list.soundData);
				}
			}
			break;
		}
		index++;
	}
	frameTime++;
}

// 音声データ解放
void Audio::SoundUnload(SoundData* soundData) {
	// バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}