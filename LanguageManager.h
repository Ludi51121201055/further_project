// languagemanager.h
#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QObject>
#include <QTranslator>

class LanguageManager : public QObject
{
    Q_OBJECT
public:
    static LanguageManager* instance();
    bool loadLanguage(const QString& langCode);
    QString currentLanguage() const;

signals:
    void languageChanged(const QString& langCode);

private:
    explicit LanguageManager(QObject *parent = nullptr);
    QTranslator* m_translator;
    QString m_currentLanguage;
};

#endif // LANGUAGEMANAGER_H

