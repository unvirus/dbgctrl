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
 
  Nintendo Switch sysmodule�p�̃f�o�b�K���C�u������API�d�l���ł��B
 
  �f�o�b�K�̎������ȑf�����邽�߂ɗ��p���邱�Ƃ��ł��܂��B
 */
 
#ifndef __DBGCTRL_H__
#define __DBGCTRL_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*! @enum	DEBUG_EVENT
    @brief	�R�[���o�b�N�C�x���g
*/
typedef enum {
    //! �^�[�Q�b�g�ڑ�
    EVT_TARGET_CONNECT = 0,
    //! �^�[�Q�b�g�ؒf
    EVT_TARGET_DISCONNECT,
    //! �u���[�N�|�C���g
    EVT_BREAK_POINT,
    //! �E�I�b�`�|�C���g
    EVT_WATCH_POINT,
    //! �\�t�g�E�G�A�u���[�N�|�C���g
    EVT_SW_BREAK_POINT,
    //! �g���b�v
    EVT_TRAP,
    //! �v���t�F�b�`�A�{�[�g
    EVT_PREFETCH_ABORT,
    //! �f�[�^�A�{�[�g 
    EVT_DATA_ABORT,
    //! �A���C�������g�t�H���g�@
    EVT_ALIGNMENT_FAULT,
    //! �ꎞ��~
    EVT_PAUSE,
} DEBUG_EVENT;

/*! @enum	WATCH_POINT_DIR
    @brief	�E�I�b�`�|�C���g����
*/
typedef enum {
    //! ���[�h����
    WATCH_POINT_READ = 0x01,
    //! ���C�g����
    WATCH_POINT_WRITE,
    //! ���[�h���C�g����
    WATCH_POINT_RW
} WATCH_POINT_DIR;

/*! @enum	WATCH_POINT_SIZE
    @brief	�E�I�b�`�|�C���g�T�C�Y
*/
typedef enum {
    //! 1�o�C�g
    WATCH_POINT_BYTE = 0x01,
    //! 2�o�C�g
    WATCH_POINT_WORD = 0x02,
    //! 4�o�C�g
    WATCH_POINT_DWORD = 0x04,
    //! 8�o�C�g
    WATCH_POINT_QWORD = 0x08
} WATCH_POINT_SIZE;

/*! @enum	DEBUG_CB_STATUS
    @brief	�R�[���o�b�N�X�e�[�^�X
*/
typedef enum {
    //! �G���[�A�f�o�b�O���f
    DEBUG_CB_ERROR = -1,
    //! �����~�ێ�
    DEBUG_CB_STOP = 0,
    //! ����ĊJ
    DEBUG_CB_CONTINUE
} DEBUG_CB_STATUS;

/*!
 @brief
    �C�x���g�R�[���o�b�N�B

    evt�̒l��EVT_BREAK_POINT�A EVT_WATCH_POINT�AEVT_SW_BREAK_POINT
 
    EVT_TRAP�AEVT_PREFETCH_ABORT�AEVT_ALIGNMENT_FAULT�AEVT_PAUSE�̎���thread_id�Aaddr�ɗL���Ȓl������B
 
    EVT_TARGET_CONNECT�AEVT_TARGET_DISCONNECT�̎���thread_id�Aaddr�͖��g�p��0�ɂȂ�B
 
    evt�̒l��EVT_BREAK_POINT�A EVT_WATCH_POINT�AEVT_SW_BREAK_POINT�̎��ADEBUG_CB_CONTINUE��Ԃ��Ə����𑱍s����B
 
    �������A�u���[�N�|�C���g�A�E�I�b�`�|�C���g��ݒ肵���܂܁ADEBUG_CB_CONTINUE��Ԃ��Ă�
 
    �����ɓ����A�h���X�ōĂуR�[���o�b�N����������B
 
    DEBUG_CB_STOP��Ԃ��Ə����͒��f�����܂܂ɂȂ�B
 
    ���̂Ƃ���ŁADcDebugContinue()���ĂԂ��Ƃŏ������ĊJ����B
 
    DEBUG_CB_ERROR��Ԃ��ƃf�o�b�O�����͒��f���ADcIsProcessing()��false��Ԃ��B
 
    arg��DcTargetConnect()�Ŏw�肵������������B
 
    �Ăяo�����̃X���b�h�̃X�^�b�N�T�C�Y��16KB�B

 @param [out]		evt �C�x���g	
 @param [out]		thread_id �X���b�hID
 @param [out]		addr �A�h���X
 @param [out]		arg �����A�I�v�V����
 
 @retval	�R�[���o�b�N�X�e�[�^�X
 */
typedef DEBUG_CB_STATUS (*DEBUG_EVENT_CALLBACK)(DEBUG_EVENT evt, u64 thread_id, u64 addr, void *arg);

/*!
 @brief

    ���C�u�����̓���ɕK�v�ȃ��W���[��������������B
 
    ���������郂�W���[����pminfoInitialize()�ApmdmntInitialize()
 
    �������Ɏ��s�����ꍇ�́A�G���[�X�N���[�����\�������B
 
    userAppInit()���ŕK���ĂԂ��ƁB
 */
void DcInit(void);

/*!
 @brief

    ���C�u�����I�����Ƀ��W���[�����J������B
 
    userAppExit()�ŕK���ĂԂ��ƁB
 
 @retval �����@ 
 */
void DcDeinit(void);

/*!
 @brief

    ���O�擾���J�n����B
 
    name�ɂ̓��O�t�@�C�����i�p�X���܂ށj���w�肷��B
 
    �t�@�C���͎����ō쐬����B���łɃt�@�C��������ꍇ�̓��O��ǋL����B
 
    �t�H���_�͑O�����č쐬���Ă����K�v������B
 
    DcLogStart()�ADcLogStop()�ADcLogPrint()�̓f�o�b�O�������ɂ����p�\�B
 
 @param [in]		name �t�@�C����
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcLogStart(const char *name);

/*!
 @brief

    ���O�擾���~����B
 
 @retval ����
 */
void DcLogStop(void);

/*!
 @brief

    ���O���o�͂���B
 
    1��̍ő啶������128�����Bprintf�Ɠ����g�������\�B
 
    ����A�L���b�V�����t���b�V������̂ŏ����͒x���B
 
 @param [in]		fmt ���O�ɏo�͂��镶����
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcLogPrint(const char *fmt, ...);

/*!
 @brief

    �A�v���P�[�V�����ɐڑ�����B
 
    priority�Ɏw��ł���l��24����59�B
 
    44����{�X���b�h�̃v���C�I���e�B�A59���ō��v���C�I���e�B�B
 
    sysmodule.json�̐ݒ�Ɉˑ�����B
 
    arg�͔C�ӂ̒l��ݒ�\�B�g�p���Ȃ��ꍇ��NULL���w�肷��B 
 
 @param [in]		tid �^�C�g��ID
 @param [in]		priority 24����59���w��
 @param [in]		callback �R�[���o�b�N�n���h��
 @param [in]		arg �����A�I�v�V����
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcTargetConnect(u64 tid, s32 priority, DEBUG_EVENT_CALLBACK callback, void *arg);

/*!
 @brief

    �A�v���P�[�V��������ؒf����B
 
    �u���[�N�|�C���g�A�E�I�b�`�|�C���g�͖����ɂȂ�B    �@
 
 @retval ���� 
 */
void DcTargetDisconnect(void);

/*!
 @brief

    ����󋵂��擾����B
 
    ���C�u���������삵�Ă���Ԃ�true��Ԃ��B
 
    timeout�ɗL���Ȓl��ݒ肵���ꍇ�A���̎��Ԃ�҂��ď�����Ԃ��B

    timeout��0���w�肷���API�͂����ɏ�����Ԃ��B  
 
 @param [in]		timeout �^�C���A�E�g�A�}�C�N���b�P��
 
 @retval	���쎞��true��Ԃ��B�����쎞��false��Ԃ��B
 */
bool DcIsProcessing(u64 timeout);

/*!
 @brief

    ���s���ĊJ����B
 
    �C�x���g�R�[���o�b�N��DEBUG_CB_STOP��Ԃ������ADcDebugPause()���s��������
 
    �A�v���P�[�V�����̎��s���ĊJ���鎞�ɌĂԁB
 
    ����ȊO�̏�Ԃ�API���Ă񂾏ꍇ�̓���͕s��B
 
    �C�x���g�R�[���o�b�N���ł͗��p�ł��Ȃ��B  
 
 @param [in]		thread_id �X���b�hID
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcDebugContinue(u64 thread_id);

/*!
 @brief
 
    �A�v���P�[�V�������ꎞ��~����B
 
    API���Ă񂾒���́A�܂��A�v���P�[�V�����͓��삵�Ă���B
 
    �A�v���P�[�V�������ꎞ��~����ƁA�C�x���g�R�[���o�b�N��EVT_PAUSE����������B
 
    ������ĊJ����ɂ́ADcDebugContinue()���ĂԁB
 
    �C�x���g�R�[���o�b�N���ł͗��p�ł��Ȃ��B  
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcDebugPause(void);

/*!
 @brief

    ��������ǂݍ��ށB
 
 @param [in]		addr �A�h���X
 @param [in]		data �ǂݍ��݃f�[�^���i�[����|�C���^
 @param [in]		len �ǂݍ��݃T�C�Y 
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcMemRead(u64 addr, void *data, u64 len);

/*!
 @brief

    �������ɏ������ށB
 
 @param [in]		addr �A�h���X
 @param [out]		data �������݃f�[�^�̃|�C���^
 @param [in]		len �������݃T�C�Y 
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcMemWrite(u64 addr, void *data, u64 len);

/*!
 @brief

    �x�[�X�A�h���X���擾����B
 
    API��������addr�ɃA�v���P�[�V�����̃x�[�X�A�h���X������B
 
 @param [out]		addr �x�[�X�A�h���X
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcGetBaseAddress(u64 *addr);

/*!
 @brief

    �v���Z�XID���擾����B
 
    API��������pid�ɃA�v���P�[�V�����̃v���Z�XID������B
 
 @param [out]		pid �v���Z�XID
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcGetPid(u64 *pid);

/*!
 @brief

    �f�o�b�O�n���h�����擾����B
 
    API��������handle�ɃA�v���P�[�V�����̃f�o�b�O�n���h��������B
 
 @param [out]		handle �f�o�b�O�n���h��
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcGetHandle(Handle *handle);

/*!
 @brief

    �v���Z�X�n���h�����擾����B
 
    API��������handle�ɃA�v���P�[�V�����̃v���Z�X�n���h��������B
 
    �v���Z�X�n���h���ƃf�o�b�O�n���h���͈قȂ�B
 
 @param [out]		handle �v���Z�X�n���h��
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcGetProcessHandle(Handle *handle);

/*!
 @brief

    �A�v���P�[�V������.text�Z�N�V�����̃T�C�Y���擾����B
 
    .text�Z�N�V�����̓v���O�����R�[�h���i�[����Ă���Z�N�V�����B
 
 @param [out]		size .text�Z�N�V�����T�C�Y
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcGetCodeAreaSize(u64 *size);

/*!
 @brief

    ���C�u�����o�[�W�������擾����B
 
    BCD�`���̒l��Ԃ��B0x0100�Ȃ��1.00�������B
 
 @retval	���C�u�����o�[�W����
 */
unsigned int DcGetVersion(void);

/*!
 @brief

    �n�[�h�E�G�A�u���[�N�|�C���g��ݒ肷��B
 
    �n�[�h�E�G�A�u���[�N�|�C���g���q�b�g����ƃR�[���o�b�N����B
 
    �n�[�h�E�G�A�u���[�N�|�C���g�𖳌��ɂ���ꍇ��addr��0���w�肷��B
 
 @param [in]		index �u���[�N�|�C���g�̃C���f�b�N�X�ԍ��A0����3
 @param [in]		addr �A�h���X 
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcBreakPoint(u32 index, u64 addr);

/*!
 @brief

    �\�t�g�E�G�A�u���[�N�|�C���g��ݒ肷��B
 
    �\�t�g�E�G�A�u���[�N�|�C���g���q�b�g����ƃR�[���o�b�N����B
 
    �\�t�g�E�G�A�u���[�N�|�C���g�𖳌��ɂ���ꍇ��addr��0���w�肷��B
 
    �\�t�g�E�G�A�u���[�N�|�C���g�́A�w��A�h���X�̖��߂��g���b�v���߂ɒu��������B
 
    �R�[�h�̈�ȊO�Ƀ\�t�g�E�G�A�u���[�N�|�C���g��ݒ肵���ꍇ�̓���͕s��ɂȂ�B
 
 @param [in]		index �u���[�N�|�C���g�̃C���f�b�N�X�ԍ��A0����31
 @param [in]		addr �A�h���X 
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcSwBreakPoint(u32 index, u64 addr);

/*!
 @brief

    �E�I�b�`�|�C���g��ݒ肷��B
 
    �E�I�b�`�|�C���g���q�b�g����ƃR�[���o�b�N����B
 
    �E�I�b�`�|�C���g�𖳌��ɂ���ꍇ��addr��0���w�肷��B
 
 @param [in]		index �E�I�b�`�|�C���g�̃C���f�b�N�X�ԍ��A0����3
 @param [in]		addr �A�h���X
 @param [in]		dir ����
 @param [in]		size �A�N�Z�X�T�C�Y
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcWatchPoint(u32 index, u64 addr, WATCH_POINT_DIR dir, WATCH_POINT_SIZE size);

/*!
 @brief

    �v���O�����R�[�h��ǂݍ��ށB
 
    addr�̓x�[�X�A�h���X����̑��΃A�h���X�Ŏw�肷��B
 
    addr��4�̔{���Ƃ��邱�ƁB�A�N�Z�X�ł���̈�̓A�v���P�[�V������.text�Z�N�V�����͈͓̔��B
 
    subsdk�̃v���O�����R�[�h��ǂݍ��ޏꍇ�́ADcMemRead()���g�p���邱�ƁB
 
    DcMemRead()��荂����API�B
 
    data�̓��g���G���f�B�A���Ȃ̂Œ��ӂ��邱�ƁB
 
 @param [in]		addr �A�h���X
 @param [out]		data �R�[�h�A1���ߕ� 
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcCodeRead(u64 addr, u32 *data);

/*!
 @brief

    �v���O�����R�[�h���������ށB
 
    addr�̓x�[�X�A�h���X����̑��΃A�h���X�Ŏw�肷��B
 
    addr��4�̔{���Ƃ��邱�ƁB�A�N�Z�X�ł���̈�̓A�v���P�[�V������.text�Z�N�V�����͈͓̔��B
 
    subsdk�Ƀv���O�����R�[�h���������ޏꍇ�́ADcMemWrite()���g�p���邱�ƁB
 
    DcMemWrite()��荂����API�B
 
    data�̓��g���G���f�B�A���Ȃ̂Œ��ӂ��邱�ƁB
 
 @param [in]		addr �A�h���X
 @param [in]		data �R�[�h�A1���ߕ�
 
 @retval	��������true��Ԃ��B���s����false��Ԃ��B
 */
bool DcCodeWrite(u64 addr, u32 *data);

/*!
 @brief

    �A�v���P�[�V�����̋N����҂B
 
    timeout�Ŏw�肵�����Ԃ�҂��āA�A�v���P�[�V�������N�����Ă����true��Ԃ��B

    timeout��0���w�肷���API�͂����ɏ�����Ԃ��B
 
    DcTargetConnect()���s���O�ɁA�A�v���P�[�V�������N�����Ă��邩���m�F���邱�Ƃ��ł���B
 
 @param [in]		tid �^�C�g��ID
 @param [in]		timeout �^�C���A�E�g�A�}�C�N���b�P��
 
 @retval	���쎞��true��Ԃ��B�����쎞��false��Ԃ��B
 */
bool DcWaitForTarget(u64 tid, u64 timeout);

#ifdef __cplusplus
}
#endif

#endif /* __DBGCTRL_H__ */

/*! @} */


