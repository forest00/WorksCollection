//conio.h�����b�s���O���܂�
//�쐬�ҁF�X�@�m�u
//�쐬���F2019/05/07
#pragma once
#include <string>

class ConIOSystem;
class ConIOSystemFactory;
class ConIOSystemWrap;

//conio.h���i��N���X�A�t�@�N�g���[���琶��
class ConIOSystem
{
	friend class ConIOSystemFactory;
public:
	//�X�V�B����Ă�
	void Update();
	//UP or W
	bool Up() const;
	//LEFT or A
	bool Left() const;
	//DOWN or S
	bool Down() const;
	//RIGHT or D
	bool Right() const;
	//SPACE or ENTER
	bool Entry() const;
	//M
	bool Menu() const;
	//V
	bool View() const;
	//C
	bool Cancel() const;
	//B
	bool Back() const;
	//P
	bool Pause() const;
	//R
	bool Resume() const;
	//E
	bool Exit() const;
	//���݉�����Ă���L�[
	int Now() const;
	//������Ŏ擾
	const std::string &UpString() const;
	const std::string &LeftString() const;
	const std::string &DownString() const;
	const std::string &RightString() const;
	const std::string &EntryString() const;
	const std::string &MenuString() const;
	const std::string &ViewString() const;
	const std::string &CancelString() const;
	const std::string &BackString() const;
	const std::string &PauseString() const;
	const std::string &ResumeString() const;
	const std::string &ExitString() const;
private:
	ConIOSystem();
	~ConIOSystem();
	int m_now_key;
	std::string m_up_string;
	std::string m_left_string;
	std::string m_down_string;
	std::string m_right_string;
	std::string m_entry_string;
	std::string m_menu_string;
	std::string m_view_string;
	std::string m_cancel_string;
	std::string m_back_string;
	std::string m_pause_string;
	std::string m_resume_string;
	std::string m_exit_string;
};

//ConIOSystemWorker�𐶐��E�p������N���X�A�����s��
class ConIOSystemFactory
{
public:
	static ConIOSystem *Create();
	static ConIOSystem *Destroy(ConIOSystem *worker);
private:
	ConIOSystemFactory();
	~ConIOSystemFactory();
	static constexpr auto m_capacity = 1;
	static int m_count;
};

//ConIOSystemWorker�����b�v
class ConIOSystemWrap
{
public:
	ConIOSystemWrap();
	~ConIOSystemWrap();
	bool IsValid() const;
	ConIOSystem &GetRef() const;
private:
	ConIOSystem *m_conio_system;
};