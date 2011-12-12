#ifdef _WIN32
    #include <nall/windows/utf8.hpp>
    void waitCallback(void *, unsigned char);
#endif

#include <phoenix/phoenix.hpp>

using namespace nall;
using namespace phoenix;

struct Application : Window {
    bool configModified;
    lstring games;
    bool engineRunning;

    VerticalLayout layMain;

    HorizontalLayout layEngine;
    Label lblEngine;
    LineEdit linEngine;
    Button btnEngine;

    VerticalLayout layGames;
    Label lblGames;
    HorizontalLayout layGamesList;
    ListView lstGames;
    VerticalLayout layAddRemove;
    Button btnAdd;
    Button btnRemove;

    Label lblGame;

    Button btnPlay;

    #ifdef _WIN32
    // *** BEGIN WINDOWS ***

    PROCESS_INFORMATION pi;
    HANDLE waitObject;
    bool waitObjectCleaned;

    void finishWait() {
        engineRunning = false;
        btnPlay.setEnabled(lstGames.selected() && linEngine.text() != "" && !engineRunning);
    }

    void cleanWaitObject() {
        if (!waitObjectCleaned) {
            UnregisterWait(waitObject);
            CloseHandle(pi.hProcess);
            waitObjectCleaned = true;
        }
    }

    void reallyRunEngine(const string& engine, const string& game, const string& script) {
        string args = {
            "\"", engine, "\"",
            " -data \"", game, "\"",
            " -main \"", script, "\""
        };
        utf16_t tmpArgs(args);

        cleanWaitObject();

        STARTUPINFO si;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        // Stop Sphere 2 from making its log file in this directory.
        string dir;
        unsigned sep = 0;
        for (unsigned n = 0; engine[n]; n++) {
            if (engine[n] == '/') sep = n;
        }
        if (sep > 0) dir = substr(engine, 0, sep);

        if (!CreateProcess(utf16_t(engine), tmpArgs, NULL, NULL, FALSE, 0, NULL, sep > 0 ? utf16_t(dir) : NULL, &si, &pi)) {
            MessageWindow::critical(*this, "Could not run engine.");
            return;
        }
        CloseHandle(pi.hThread);

        if (RegisterWaitForSingleObject(&waitObject, pi.hProcess, (WAITORTIMERCALLBACK)waitCallback, NULL, INFINITE, WT_EXECUTEONLYONCE)) {
            engineRunning = true;
            btnPlay.setEnabled(false);
            waitObjectCleaned = false;
        }
    }

    // *** END WINDOWS ***
    #else
    // *** BEGIN NOT WINDOWS ***

    void reallyRunEngine(const string& engine, const string& game, const string& script) {
        string args = {
            "\"", engine, "\"",
            " -data \"", game, "\"",
            " -main \"", script, "\""
        };

        MessageWindow::information(*this, { engine, "\n", args });
    }

    // *** END NOT WINDOWS ***
    #endif

    void runEngine(const string& engine, const string& game) {
        if (engine == "" || game == "") return;

        unsigned sep = 0;
        unsigned ext = 0;
        for (unsigned n = 0; game[n]; n++) {
            if (game[n] == '/') sep = n;
            else if (game[n] == '.') ext = n;
        }
        if (sep == 0 || ext == 0) return;

        reallyRunEngine(engine, substr(game, 0, sep), substr(game, sep + 1, ext - sep - 1));
    }

    void updateGameButtons() {
        bool selected = lstGames.selected();
        btnRemove.setEnabled(selected);
        btnPlay.setEnabled(selected && linEngine.text() != "" && !engineRunning);
        if (selected) {
            auto sel = lstGames.selection();
            lblGame.setText(games[sel]);
        } else {
            lblGame.setText("");
        }
    }

    void updateGames() {
        lstGames.reset();
        for (auto &g : games) {
            unsigned prev = 0;
            unsigned curr = 0;
            for (unsigned n = 0; g[n]; n++) {
                if (g[n] == '/') {
                    prev = curr;
                    curr = n;
                }
            }
            if (curr > 0) {
                lstGames.append(substr(g, prev + 1));
            } else {
                lstGames.append(g);
            }
        }
        updateGameButtons();
    }

    void readConfig() {
        string config;
        if (config.readfile({ userpath(), "s2launch.txt" })) {
            lstring tmp;
            tmp.split("\n", config);
            if (tmp.size() > 0) linEngine.setText(tmp[0].rtrim("\n"));
            auto t = tmp.begin();
            for (t++; t != tmp.end(); t++) games.append(t->rtrim("\n"));
        }
    }

    void writeConfig() {
        file f;
        if (f.open({ userpath(), "s2launch.txt" }, file::mode::write)) {
            f.print(linEngine.text());
            for (auto &g : games) f.print("\n", g);
            f.close();
        }
    }

    void create() {
        configModified = false;
        engineRunning = false;
        #ifdef _WIN32
        waitObjectCleaned = true;
        #endif

        readConfig();

        setTitle("Sphere 2 Launcher");
        setGeometry({ 300, 100, 400, 300 });

        lblEngine.setText("Sphere 2 engine:");
        linEngine.setEditable(false);
        btnEngine.setText("Open...");

        lblGames.setText("Games:");
        updateGames();
        btnAdd.setText("Add...");
        btnRemove.setText("Remove");
        btnRemove.setEnabled(false);

        btnPlay.setText("Play!");
        btnPlay.setEnabled(false);

        layMain.setMargin(4);

        layEngine.append(lblEngine, 0, 0, 8);
        layEngine.append(linEngine, ~0, 0, 8);
        layEngine.append(btnEngine, 0, 0);
        layMain.append(layEngine, ~0, 0, 8);

        layGames.append(lblGames, ~0, 0, 8);
        layGamesList.append(lstGames, ~0, ~0, 8);
        layAddRemove.append(btnAdd, ~0, 0, 8);
        layAddRemove.append(btnRemove, 0, 0);
        layGamesList.append(layAddRemove, 0, 0);
        layGames.append(layGamesList, ~0, ~0);
        layMain.append(layGames, ~0, ~0, 8);

        layMain.append(lblGame, ~0, 0, 8);
        layMain.append(btnPlay, ~0, 0);

        append(layMain);

        onClose = [this]() {
            #ifdef _WIN32
            cleanWaitObject();
            #endif
            if (configModified) writeConfig();
            OS::quit();
        };

        btnEngine.onTick = [this]() {
            string location = OS::fileLoad(*this, "", "Programs (*.exe)", "All files (*)");
            if (location != "") {
                linEngine.setText(location);
                configModified = true;
            }
            updateGameButtons();
        };

        lstGames.onActivate = lstGames.onChange = [this]() {
            updateGameButtons();
        };

        btnAdd.onTick = [this]() {
            string game = OS::fileLoad(*this, "", "Sphere 2 games (*.script, *.sqrl)", "All files (*)");
            if (game != "") {
                games.append(game);
                updateGames();
                lstGames.setSelection(games.size() - 1);
                lstGames.setFocused();
                updateGameButtons();
                configModified = true;
            }
        };

        btnRemove.onTick = [this]() {
            if (!lstGames.selected()) return;
            auto sel = lstGames.selection();
            auto resp = MessageWindow::question(*this, {"Remove ", games[sel], "?"});
            if (resp == MessageWindow::Response::Yes) {
                games.remove(sel);
                updateGames();
                configModified = true;
            }
        };

        btnPlay.onTick = [this]() {
            if (linEngine.text() == "" || !lstGames.selected()) return;
            runEngine(linEngine.text(), games[lstGames.selection()]);
        };

        setVisible();
    }
} application;

#ifdef _WIN32
void waitCallback(void *param, unsigned char timedOut) {
    application.finishWait();
}
#endif

int main() {
    application.create();
    OS::main();
    return 0;
}