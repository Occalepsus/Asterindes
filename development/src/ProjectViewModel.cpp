#include "ProjectViewModel.h"

using namespace Asterindes::Ui;

ProjectViewModel::ProjectViewModel(QPointer<AsterindesProject> p_projectManager, QObject* p_parent)
	: QObject(p_parent)
	, m_projectManager(p_projectManager)
{

}

void ProjectViewModel::loadProject(const QUrl& p_projectPath)
{
}

void ProjectViewModel::onProjectLoaded(const QUrl& p_projectPath)
{

}