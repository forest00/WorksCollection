#include "conio_system.h"
#include <assert.h>
#include <conio.h>

//ConIOSystemWorker
ConIOSystem::ConIOSystem() :
	m_now_key(0),
	m_up_string("UP or W"),
	m_left_string("LEFT or A"),
	m_down_string("DOWN or S"),
	m_right_string("RIGHT or D"),
	m_entry_string("SPACE or ENTER"),
	m_menu_string("M"),
	m_view_string("V"),
	m_cancel_string("C"),
	m_back_string("B"),
	m_pause_string("P"),
	m_resume_string("R"),
	m_exit_string("E")
{

}
ConIOSystem::~ConIOSystem()
{
	//“Á‚É‚â‚é‚±‚Æ‚Í‚È‚¢
}
void ConIOSystem::Update()
{
	m_now_key = 0;
	while (_kbhit())
	{
		m_now_key = _getch();
	}
}
bool ConIOSystem::Up() const
{
	return m_now_key == 'w' || m_now_key == 72;
}
bool ConIOSystem::Left() const
{
	return m_now_key == 'a' || m_now_key == 75;
}
bool ConIOSystem::Down() const
{
	return m_now_key == 's' || m_now_key == 80;
}
bool ConIOSystem::Right() const
{
	return m_now_key == 'd' || m_now_key == 77;
}
bool ConIOSystem::Entry() const
{
	return m_now_key == ' ' || m_now_key == 13;
}
bool ConIOSystem::Menu() const
{
	return m_now_key == 'm';
}
bool ConIOSystem::View() const
{
	return m_now_key == 'v';
}
bool ConIOSystem::Cancel() const
{
	return m_now_key == 'c';
}
bool ConIOSystem::Back() const
{
	return m_now_key == 'b';
}
bool ConIOSystem::Pause() const
{
	return m_now_key == 'p';
}
bool ConIOSystem::Resume() const
{
	return m_now_key == 'r';
}
bool ConIOSystem::Exit() const
{
	return m_now_key == 'e';
}
int ConIOSystem::Now() const
{
	return m_now_key;
}
const std::string &ConIOSystem::UpString() const
{
	return m_up_string;
}
const std::string &ConIOSystem::LeftString() const
{
	return m_left_string;
}
const std::string &ConIOSystem::DownString() const
{
	return m_down_string;
}
const std::string &ConIOSystem::RightString() const
{
	return m_right_string;
}
const std::string &ConIOSystem::EntryString() const
{
	return m_entry_string;
}
const std::string &ConIOSystem::MenuString() const
{
	return m_menu_string;
}
const std::string &ConIOSystem::ViewString() const
{
	return m_view_string;
}
const std::string &ConIOSystem::CancelString() const
{
	return m_cancel_string;
}
const std::string &ConIOSystem::BackString() const
{
	return m_back_string;
}
const std::string &ConIOSystem::PauseString() const
{
	return m_pause_string;
}
const std::string &ConIOSystem::ResumeString() const
{
	return m_resume_string;
}
const std::string &ConIOSystem::ExitString() const
{
	return m_exit_string;
}

//ConIOSystemWorkerFactory
ConIOSystemFactory::ConIOSystemFactory()
{
	//‚±‚ÌƒNƒ‰ƒX‚ª¶¬‚³‚ê‚é‚±‚Æ‚Í‚È‚¢‚Ì‚Å“Á‚É‚â‚é‚±‚Æ‚Í‚È‚¢
}
ConIOSystemFactory::~ConIOSystemFactory()
{
	//“Á‚É‚â‚é‚±‚Æ‚Í‚È‚¢
}
ConIOSystem *ConIOSystemFactory::Create()
{
	if (m_count >= m_capacity)
	{
		return nullptr;
	}
	m_count++;
	return new ConIOSystem;
}
ConIOSystem *ConIOSystemFactory::Destroy(ConIOSystem *worker)
{
	delete worker;
	m_count--;
	return nullptr;
}
int ConIOSystemFactory::m_count = 0;

//ConIOSystem
ConIOSystemWrap::ConIOSystemWrap() :
	m_conio_system(ConIOSystemFactory::Create())
{
}
ConIOSystemWrap::~ConIOSystemWrap()
{
	m_conio_system = ConIOSystemFactory::Destroy(m_conio_system);
}
bool ConIOSystemWrap::IsValid() const
{
	return m_conio_system == nullptr;
}
ConIOSystem &ConIOSystemWrap::GetRef() const
{
	if (IsValid())
	{
		assert(0);
	}
	return *m_conio_system;
}
