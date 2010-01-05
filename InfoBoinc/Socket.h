/*
 * =====================================================================================
 *
 *       Filename:  Socket.h
 *
 *        Version:  1.0
 *        Created:  31.12.2009 14:56:34
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 *
 * =====================================================================================
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QSemaphore>
#include <QtCore/QThread>
#include <QtNetwork/QTcpSocket>

namespace InfoBoinc {

/**
 * \class Socket
 * \brief %Socket pre pripojenie sa k vzdialenému, alebo lokálnemu klientovi BOINC.
 * \ingroup InfoBoinc
 */
class Socket: public QThread
{
Q_OBJECT
Q_ENUMS(State)
/**
 * Približný stav socketu (nemusí byť 100% správny kvôli tomu, že so socketom sa
 * pracuje v inom vlákne).
 */
Q_PROPERTY(State state READ state NOTIFY stateChanged)
public:
	/**
	 * Vytvorenie nového socketu pre komunikáciu s BOINC.
	 */
	Socket(QObject *parent = 0);

	/**
	 * Zrušenie socketu a prípadné odpojenie od klienta.
	 */
	~Socket();

	/**
	 * \brief Pripojenie sa ku klientovi.
	 * Volaním tejto metódy sa ukončí doteraz otvorené spojenie (ak nejaké
	 * existovalo) a vytvorí sa nové spojenie s klientom na stroji \a host
	 * počúvajúcom na porte \a port.
	 */
	void connectToBoinc(const QString &host, quint16 port);

	/**
	 * \brief Odpojenie sa od klienta.
	 * Volaním tejto funkcie sa zastavia prebiehajúce úlohy a Socket::dataRecived
	 * bude emitovať \e null hodnoty namiesto výsledkov volania.
	 */
	void disconnectFromBoinc();

	/**
	 * Stav pripojenia ku klientovi.
	 */
	enum State {
		UnconnectedState,  /**< Odpojený od klienta.        */
		ConnectingState,   /**< Pripájanie sa ku klientovi. */
		ConnectedState,    /**< Pripojený.                  */
		DisconnectingState /**< Odpájanie sa od klienta.    */
	};

	/**
	 * Vráti stav pripojenia ku klientovi.
	 */
	State state();

	/**
	 * Odoslanie dát \a data BOINC klientovi. Odosielanie a príjem dát fungujú
	 * acynchrónne, preto funkcia nevracia odpoveď klienta, ani žiaden chybový
	 * kód. V prípade vyskytnutia sa chyby emituje Socket::dataRecived \e null
	 * hodnotu. Odpovede prichádzajú v rovnakom poradí, v a kom boli zaslané
	 * dotazy.
	 */
	void sendData(const QByteArray &data);

signals:
	/**
	 * Tento signál sa emituje pri zmene stavu socketu. Pretože so socketom sa
	 * pracuje v inom vlákne nemusí zodpovedať emitovaný stav aktuálnemu stavu
	 * socketu.
	 */
	void stateChanged(Socket::State state);

	/**
	 * Pri prijatí dát sa emituje tento signál. Ak došlo k chybe pri prenose
	 * emituje sa namiesto prijatých dát \e null. V prípade, že socket bol
	 * odpojený a vo vstupnom bufferi ešte ostali neodoslané správy pre každú
	 * túto správu sa emituje \e null odpoveď podobne ako keby došlo k chybe pri
	 * prenose.
	 */
	void dataRecived(const QByteArray &data);

	/**
	 * Tento signál sa emituje ak nastala chyba socketu. Reťazec \a errorMsg
	 * obsahuje chybovú správu socketu.
	 */
	void error(const QString &errorMsg);

private slots:
	void updateSocketState(QAbstractSocket::SocketState socketState);
	void emitError();
	void setFinishedState();

private:
	void clearInputBuffer();
	virtual void run();

	static const unsigned long ConnectWait = 30000;
	static const unsigned long ReadWait = 10000;
	static const unsigned long DisconnectWait = 2000;

	/* ====================  DATA MEMBERS  ==================== */
	QString m_host;
	quint16 m_port;
	QTcpSocket *m_socket;
	State m_state;
	QSemaphore m_msgFrontLength;
	QMutex m_msgFrontMutex;
	QMutex m_sockMutex;
	QList<QByteArray> m_msgFront;
}; /* -----  end of class Socket  ----- */

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, Socket::State state);
#endif

} /* end of namespace InfoBoinc */

#endif    /* ----- #ifndef SOCKET_H  ----- */

