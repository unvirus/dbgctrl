/* 
 * dbgctrl.h
 *
 * Debug Library header
 * Copyright (c) 2022-2024 unvirus
 * Released under the MIT license
 *
 */

/*!
 @file	dbgctrl.h
 @brief	Debug Library header file

 @author  unvirus
 @date    2023/12/01
*/

/*! @addtogroup dbgctrl */
/*! @{ */

/*! 
  @mainpage dbgctrl
 
  Nintendo Switch sysmodule用のデバッガライブラリのAPI仕様書です。
 
  デバッガの実装を簡素化するために利用することができます。
 */
 
#ifndef __DBGCTRL_H__
#define __DBGCTRL_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*! @enum	DEBUG_EVENT
    @brief	コールバックイベント
*/
typedef enum {
    //! ターゲット接続
    EVT_TARGET_CONNECT = 0,
    //! ターゲット切断
    EVT_TARGET_DISCONNECT,
    //! ブレークポイント
    EVT_BREAK_POINT,
    //! ウオッチポイント
    EVT_WATCH_POINT,
    //! ソフトウエアブレークポイント
    EVT_SW_BREAK_POINT,
    //! トラップ
    EVT_TRAP,
    //! プリフェッチアボート
    EVT_PREFETCH_ABORT,
    //! データアボート 
    EVT_DATA_ABORT,
    //! アラインメントフォルト　
    EVT_ALIGNMENT_FAULT,
    //! 一時停止
    EVT_PAUSE,
} DEBUG_EVENT;

/*! @enum	WATCH_POINT_DIR
    @brief	ウオッチポイント方向
*/
typedef enum {
    //! リード方向
    WATCH_POINT_READ = 0x01,
    //! ライト方向
    WATCH_POINT_WRITE,
    //! リードライト方向
    WATCH_POINT_RW
} WATCH_POINT_DIR;

/*! @enum	WATCH_POINT_SIZE
    @brief	ウオッチポイントサイズ
*/
typedef enum {
    //! 1バイト
    WATCH_POINT_BYTE = 0x01,
    //! 2バイト
    WATCH_POINT_WORD = 0x02,
    //! 4バイト
    WATCH_POINT_DWORD = 0x04,
    //! 8バイト
    WATCH_POINT_QWORD = 0x08
} WATCH_POINT_SIZE;

/*! @enum	DEBUG_CB_STATUS
    @brief	コールバックステータス
*/
typedef enum {
    //! エラー、デバッグ中断
    DEBUG_CB_ERROR = -1,
    //! 動作停止維持
    DEBUG_CB_STOP = 0,
    //! 動作再開
    DEBUG_CB_CONTINUE
} DEBUG_CB_STATUS;

/*!
 @brief
    イベントコールバック。

    evtの値がEVT_BREAK_POINT、 EVT_WATCH_POINT、EVT_SW_BREAK_POINT
 
    EVT_TRAP、EVT_PREFETCH_ABORT、EVT_ALIGNMENT_FAULT、EVT_PAUSEの時はthread_id、addrに有効な値が入る。
 
    EVT_TARGET_CONNECT、EVT_TARGET_DISCONNECTの時はthread_id、addrは未使用で0になる。
 
    evtの値がEVT_BREAK_POINT、 EVT_WATCH_POINT、EVT_SW_BREAK_POINTの時、DEBUG_CB_CONTINUEを返すと処理を続行する。
 
    ただし、ブレークポイント、ウオッチポイントを設定したまま、DEBUG_CB_CONTINUEを返しても
 
    すぐに同じアドレスで再びコールバックが発生する。
 
    DEBUG_CB_STOPを返すと処理は中断したままになる。
 
    このとき後で、DcDebugContinue()を呼ぶことで処理を再開する。
 
    DEBUG_CB_ERRORを返すとデバッグ処理は中断し、DcIsProcessing()はfalseを返す。
 
    argはDcTargetConnect()で指定した引数が入る。
 
    呼び出し元のスレッドのスタックサイズは16KB。

 @param [out]		evt イベント	
 @param [out]		thread_id スレッドID
 @param [out]		addr アドレス
 @param [out]		arg 引数、オプション
 
 @retval	コールバックステータス
 */
typedef DEBUG_CB_STATUS (*DEBUG_EVENT_CALLBACK)(DEBUG_EVENT evt, u64 thread_id, u64 addr, void *arg);

/*!
 @brief

    ライブラリの動作に必要なモジュールを初期化する。
 
    初期化するモジュールはpminfoInitialize()、pmdmntInitialize()
 
    初期化に失敗した場合は、エラースクリーンが表示される。
 
    userAppInit()内で必ず呼ぶこと。
 */
void DcInit(void);

/*!
 @brief

    ライブラリ終了時にモジュールを開放する。
 
    userAppExit()で必ず呼ぶこと。
 
 @retval 無し　 
 */
void DcDeinit(void);

/*!
 @brief

    ログ取得を開始する。
 
    nameにはログファイル名（パスを含む）を指定する。
 
    ファイルは自動で作成する。すでにファイルがある場合はログを追記する。
 
    フォルダは前もって作成しておく必要がある。
 
    DcLogStart()、DcLogStop()、DcLogPrint()はデバッグ無効時にも利用可能。
 
 @param [in]		name ファイル名
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcLogStart(const char *name);

/*!
 @brief

    ログ取得を停止する。
 
 @retval 無し
 */
void DcLogStop(void);

/*!
 @brief

    ログを出力する。
 
    1回の最大文字数は128文字。printfと同じ使い方が可能。
 
    毎回、キャッシュをフラッシュするので処理は遅い。
 
 @param [in]		fmt ログに出力する文字列
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcLogPrint(const char *fmt, ...);

/*!
 @brief

    アプリケーションに接続する。
 
    priorityに指定できる値は24から59。
 
    44が基本スレッドのプライオリティ、59が最高プライオリティ。
 
    sysmodule.jsonの設定に依存する。
 
    argは任意の値を設定可能。使用しない場合はNULLを指定する。 
 
 @param [in]		tid タイトルID
 @param [in]		priority 24から59を指定
 @param [in]		callback コールバックハンドラ
 @param [in]		arg 引数、オプション
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcTargetConnect(u64 tid, s32 priority, DEBUG_EVENT_CALLBACK callback, void *arg);

/*!
 @brief

    アプリケーションから切断する。
 
    ブレークポイント、ウオッチポイントは無効になる。    　
 
 @retval 無し 
 */
void DcTargetDisconnect(void);

/*!
 @brief

    動作状況を取得する。
 
    ライブラリが動作している間はtrueを返す。
 
    timeoutに有効な値を設定した場合、その時間を待って処理を返す。

    timeoutに0を指定するとAPIはすぐに処理を返す。  
 
 @param [in]		timeout タイムアウト、マイクロ秒単位
 
 @retval	動作時はtrueを返す。未動作時はfalseを返す。
 */
bool DcIsProcessing(u64 timeout);

/*!
 @brief

    実行を再開する。
 
    イベントコールバックでDEBUG_CB_STOPを返した時、DcDebugPause()を行った時に
 
    アプリケーションの実行を再開する時に呼ぶ。
 
    それ以外の状態でAPIを呼んだ場合の動作は不定。
 
    イベントコールバック内では利用できない。  
 
 @param [in]		thread_id スレッドID
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcDebugContinue(u64 thread_id);

/*!
 @brief
 
    アプリケーションを一時停止する。
 
    APIを呼んだ直後は、まだアプリケーションは動作している。
 
    アプリケーションが一時停止すると、イベントコールバックでEVT_PAUSEが発生する。
 
    動作を再開するには、DcDebugContinue()を呼ぶ。
 
    イベントコールバック内では利用できない。  
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcDebugPause(void);

/*!
 @brief

    メモリを読み込む。
 
 @param [in]		addr アドレス
 @param [in]		data 読み込みデータを格納するポインタ
 @param [in]		len 読み込みサイズ 
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcMemRead(u64 addr, void *data, u64 len);

/*!
 @brief

    メモリに書き込む。
 
 @param [in]		addr アドレス
 @param [out]		data 書き込みデータのポインタ
 @param [in]		len 書き込みサイズ 
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcMemWrite(u64 addr, void *data, u64 len);

/*!
 @brief

    ベースアドレスを取得する。
 
    API成功時にaddrにアプリケーションのベースアドレスが入る。
 
 @param [out]		addr ベースアドレス
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcGetBaseAddress(u64 *addr);

/*!
 @brief

    プロセスIDを取得する。
 
    API成功時にpidにアプリケーションのプロセスIDが入る。
 
 @param [out]		pid プロセスID
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcGetPid(u64 *pid);

/*!
 @brief

    デバッグハンドルを取得する。
 
    API成功時にhandleにアプリケーションのデバッグハンドルが入る。
 
 @param [out]		handle デバッグハンドル
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcGetHandle(Handle *handle);

/*!
 @brief

    プロセスハンドルを取得する。
 
    API成功時にhandleにアプリケーションのプロセスハンドルが入る。
 
    プロセスハンドルとデバッグハンドルは異なる。
 
 @param [out]		handle プロセスハンドル
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcGetProcessHandle(Handle *handle);

/*!
 @brief

    アプリケーションの.textセクションのサイズを取得する。
 
    .textセクションはプログラムコードが格納されているセクション。
 
 @param [out]		size .textセクションサイズ
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcGetCodeAreaSize(u64 *size);

/*!
 @brief

    ライブラリバージョンを取得する。
 
    BCD形式の値を返す。0x0100ならば1.00を示す。
 
 @retval	ライブラリバージョン
 */
unsigned int DcGetVersion(void);

/*!
 @brief

    ハードウエアブレークポイントを設定する。
 
    ハードウエアブレークポイントがヒットするとコールバックする。
 
    ハードウエアブレークポイントを無効にする場合はaddrに0を指定する。
 
 @param [in]		index ブレークポイントのインデックス番号、0から3
 @param [in]		addr アドレス 
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcBreakPoint(u32 index, u64 addr);

/*!
 @brief

    ソフトウエアブレークポイントを設定する。
 
    ソフトウエアブレークポイントがヒットするとコールバックする。
 
    ソフトウエアブレークポイントを無効にする場合はaddrに0を指定する。
 
    ソフトウエアブレークポイントは、指定アドレスの命令をトラップ命令に置き換える。
 
    コード領域以外にソフトウエアブレークポイントを設定した場合の動作は不定になる。
 
 @param [in]		index ブレークポイントのインデックス番号、0から31
 @param [in]		addr アドレス 
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcSwBreakPoint(u32 index, u64 addr);

/*!
 @brief

    ウオッチポイントを設定する。
 
    ウオッチポイントがヒットするとコールバックする。
 
    ウオッチポイントを無効にする場合はaddrに0を指定する。
 
 @param [in]		index ウオッチポイントのインデックス番号、0から3
 @param [in]		addr アドレス
 @param [in]		dir 方向
 @param [in]		size アクセスサイズ
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcWatchPoint(u32 index, u64 addr, WATCH_POINT_DIR dir, WATCH_POINT_SIZE size);

/*!
 @brief

    プログラムコードを読み込む。
 
    addrはベースアドレスからの相対アドレスで指定する。
 
    addrは4の倍数とすること。アクセスできる領域はアプリケーションの.textセクションの範囲内。
 
    subsdkのプログラムコードを読み込む場合は、DcMemRead()を使用すること。
 
    DcMemRead()より高速なAPI。
 
    dataはリトルエンディアンなので注意すること。
 
 @param [in]		addr アドレス
 @param [out]		data コード、1命令分 
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcCodeRead(u64 addr, u32 *data);

/*!
 @brief

    プログラムコードを書き込む。
 
    addrはベースアドレスからの相対アドレスで指定する。
 
    addrは4の倍数とすること。アクセスできる領域はアプリケーションの.textセクションの範囲内。
 
    subsdkにプログラムコードを書き込む場合は、DcMemWrite()を使用すること。
 
    DcMemWrite()より高速なAPI。
 
    dataはリトルエンディアンなので注意すること。
 
 @param [in]		addr アドレス
 @param [in]		data コード、1命令分
 
 @retval	成功時はtrueを返す。失敗時はfalseを返す。
 */
bool DcCodeWrite(u64 addr, u32 *data);

/*!
 @brief

    アプリケーションの起動を待つ。
 
    timeoutで指定した時間を待って、アプリケーションが起動していればtrueを返す。

    timeoutに0を指定するとAPIはすぐに処理を返す。
 
    DcTargetConnect()を行う前に、アプリケーションが起動しているかを確認することができる。
 
 @param [in]		tid タイトルID
 @param [in]		timeout タイムアウト、マイクロ秒単位
 
 @retval	動作時はtrueを返す。未動作時はfalseを返す。
 */
bool DcWaitForTarget(u64 tid, u64 timeout);

#ifdef __cplusplus
}
#endif

#endif /* __DBGCTRL_H__ */

/*! @} */


