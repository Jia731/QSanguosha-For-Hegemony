/********************************************************************
    Copyright (c) 2013-2014 - QSanguosha-Hegemony Team

    This file is part of QSanguosha-Hegemony.

    This game is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3.0 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    See the LICENSE file for more details.

    QSanguosha-Hegemony Team
    *********************************************************************/

#include "miniscenarios.h"

#include <QMessageBox>
#include <QFile>

const char *MiniScene::S_KEY_MINISCENE = "_mini_%1";
const char *MiniSceneRule::S_EXTRA_OPTION_RANDOM_ROLES = "randomRoles";
const char *MiniSceneRule::S_EXTRA_OPTION_REST_IN_DISCARD_PILE = "restInDiscardPile";
const QString MiniSceneRule::_S_DEFAULT_HERO = "sujiang";

MiniSceneRule::MiniSceneRule(Scenario *scenario)
    : ScenarioRule(scenario)
{
    events << GameStart << EventPhaseStart << FetchDrawPileCard;
}

void MiniSceneRule::assign(QStringList &generals, QStringList &generals2, QStringList &kingdoms, Room *) const{
    for (int i = 0; i < players.length(); i++) {
        QMap<QString, QString> sp = players.at(i);
        QString name = sp["general"];
        QString name2 = sp["general2"];
        if (name == "select") name = _S_DEFAULT_HERO;
        if (name2 == "select") name2 = _S_DEFAULT_HERO;
        generals << name;
        generals2 << name2;
        QString k = sp["nationality"].isEmpty() ? "default" : sp["nationality"];
        kingdoms << k;
    }
}

bool MiniSceneRule::effect(TriggerEvent triggerEvent, Room *room, ServerPlayer *player, QVariant &, ServerPlayer *) const{
    if (triggerEvent == EventPhaseStart) {
        if (player == room->getTag("Starter").value<ServerPlayer *>()) {
            if (player->getPhase() == Player::Start) {
                room->setTag("Round", room->getTag("Round").toInt() + 1);

                if (!ex_options["beforeStartRound"].isNull()) {
                    if (ex_options["beforeStartRound"].toInt() == room->getTag("Round").toInt())
                        room->gameOver(ex_options["beforeStartRoundWinner"].toString());
                }
            }
            else if (player->getPhase() == Player::NotActive) {
                if (!ex_options["afterRound"].isNull()) {
                    if (ex_options["afterRound"].toInt() == room->getTag("Round").toInt())
                        room->gameOver(ex_options["afterRoundWinner"].toString());
                }
            }
        }

        if (player->getPhase() == Player::RoundStart && this->players.first()["beforeNext"] != QString()) {
            if (player->tag["playerHasPlayed"].toBool())
                room->gameOver(this->players.first()["beforeNext"]);
            else player->tag["playerHasPlayed"] = true;
        }

        if (player->getPhase() != Player::NotActive) return false;
        if (player->getState() == "robot" || this->players.first()["singleTurn"] == QString())
            return false;
        room->gameOver(this->players.first()["singleTurn"]);
        return true;
    }
    else if (triggerEvent == FetchDrawPileCard) {
        if (this->players.first()["endedByPile"] != QString()) {
            const QList<int> &drawPile = room->getDrawPile();
            foreach(int id, m_fixedDrawCards) {
                if (drawPile.contains(id))
                    return false;
            }
            room->gameOver(this->players.first()["endedByPile"]);
            return true;
        }
        return false;
    }
    else if (triggerEvent == GameStart) {
        if (room->getTag("WaitForPlayer").toBool())
            return true;

        /*
        if (objectName().startsWith("_mini_")) {
        room->doLightbox(objectName(), 2000);

        LogMessage log;
        log.type = "#WelcomeToMiniScenario";
        log.arg = objectName().mid(6);
        log.arg2 = objectName();
        room->sendLog(log);
        }
        */

        QList<ServerPlayer *> players = room->getAllPlayers();
        while (players.first()->getState() == "robot")
            players.append(players.takeFirst());

        QList<int> &drawPile = room->getDrawPile();

        foreach(int id, m_fixedDrawCards) {
            if (drawPile.contains(id)) {
                drawPile.removeOne(id);
                drawPile.prepend(id);
            }
            else {
                room->moveCardTo(Sanguosha->getCard(id), NULL, Player::DrawPile, true);
            }
            room->addPlayerHistory(NULL, "pushPile");
        }
        if (m_fixedDrawCards.length() > 0 && ex_options.contains(S_EXTRA_OPTION_REST_IN_DISCARD_PILE)) {
            DummyCard dummy;
            foreach(int id, drawPile) {
                if (!m_fixedDrawCards.contains(id))
                    dummy.addSubcard(id);
            }
            room->moveCardTo(&dummy, NULL, Player::DiscardPile);
        }

        QList<int> int_list;
        for (int i = 0; i < players.length(); i++)
            int_list << i;
        if (ex_options.contains(S_EXTRA_OPTION_RANDOM_ROLES))
            qShuffle(int_list);

        QStringList all = Sanguosha->getRandomGenerals(Sanguosha->getGeneralCount());
        qShuffle(all);
        for (int i = 0; i < players.length(); i++) {
            QString general = this->players[i]["general"];
            if (!general.isEmpty() && general != "select") all.removeOne(general);
            general = this->players[i]["general2"];
            if (!general.isEmpty() && general != "select") all.removeOne(general);
        }
        for (int j = 0; j < players.length(); j++) {
            int i = int_list[j];
            ServerPlayer *sp = players.at(j);

            QString general = this->players[i]["general"];
            if (general == "select") {
                QStringList available;
                for (int k = 0; k < 5; k++) {
                    if (sp->getGeneral()) {
                        foreach(const Skill *skill, sp->getGeneral()->getSkillList(true, true))
                            sp->loseSkill(skill->objectName());
                    }
                    QString choice = sp->findReasonable(all);
                    available << choice;
                    all.removeOne(choice);
                }
                general = room->askForGeneral(sp, available);
                all.append(available);
                all.removeOne(general);
                qShuffle(all);
                sp->setActualGeneral1Name(general);
                foreach(const Skill *skill, sp->getActualGeneral1()->getSkillList())
                    sp->addSkill(skill->objectName());
                room->notifyProperty(sp, sp, "actual_general1");;
                room->notifyProperty(sp, sp, "general", general);

            }

            general = this->players[i]["general2"];
            if (!general.isEmpty()) {
                if (general == "select") {
                    QStringList available;
                    for (int k = 0; k < 5; k++) {
                        if (sp->getGeneral2()) {
                            foreach(const Skill *skill, sp->getGeneral2()->getSkillList(true, false))
                                sp->loseSkill(skill->objectName());
                        }
                        room->setPlayerProperty(sp, "general2", QVariant());
                        QString choice = sp->findReasonable(all);
                        available << choice;
                        all.removeOne(choice);
                    }
                    general = room->askForGeneral(sp, available);
                    all.append(available);
                    all.removeOne(general);
                    qShuffle(all);
                    sp->setActualGeneral2Name(general);
                    foreach(const Skill *skill, sp->getActualGeneral2()->getSkillList())
                        sp->addSkill(skill->objectName(), false);
                    room->notifyProperty(sp, sp, "actual_general2");
                    room->notifyProperty(sp, sp, "general2", general);
                }
            }

            room->setPlayerProperty(sp, "kingdom", sp->getActualGeneral1()->getKingdom());

            QString str = this->players.at(i)["maxhp"];
            if (str == QString()) str = QString::number(sp->getGeneralMaxHp());
            sp->setMaxHp(str.toInt());
            room->broadcastProperty(sp, "maxhp");

            str = this->players.at(i)["hpadj"];
            if (str != QString()) {
                sp->setMaxHp(sp->getMaxHp() + str.toInt());
                room->broadcastProperty(sp, "maxhp");
            }

            str = QString::number(sp->getMaxHp());
            QString str2 = this->players.at(i)["hp"];
            if (str2 != QString()) str = str2;
            sp->setHp(str.toInt());
            room->broadcastProperty(sp, "hp");

            room->setTag("FirstRound", true);
            str = this->players.at(i)["equip"];
            QStringList equips = str.split(",");
            foreach(QString equip, equips) {
                bool ok;
                equip.toInt(&ok);
                if (!ok)
                    room->installEquip(sp, equip);
                else
                    room->moveCardTo(Sanguosha->getCard(equip.toInt()), NULL, sp,
                    Player::PlaceEquip, CardMoveReason(CardMoveReason::S_REASON_UNKNOWN, QString()));
            }

            str = this->players.at(i)["judge"];
            if (str != QString()) {
                QStringList judges = str.split(",");
                foreach(QString judge, judges)
                    room->moveCardTo(Sanguosha->getCard(judge.toInt()), NULL, sp,
                    Player::PlaceDelayedTrick, CardMoveReason(CardMoveReason::S_REASON_UNKNOWN, QString()));
            }

            str = this->players.at(i)["hand"];
            if (str != QString()) {
                QStringList hands = str.split(",");
                DummyCard dummy(StringList2IntList(hands));
                room->obtainCard(sp, &dummy);
            }
            room->setTag("FirstRound", false);

            QString skills = this->players.at(i)["acquireSkills"];
            if (skills != QString()) {
                foreach(QString skill_name, skills.split(","))
                    room->acquireSkill(sp, skill_name);
            }

            if (this->players.at(i)["chained"] != QString()) {
                sp->setChained(true);
                room->broadcastProperty(sp, "chained");
                room->setEmotion(sp, "chain");
            }

            if (this->players.at(i)["turned"] != QString()) {
                if (sp->faceUp())
                    sp->turnOver();
            }

            if (this->players.at(i)["starter"] != QString()) {
                room->setCurrent(sp);
                QVariant data = QVariant::fromValue(sp);
                room->setTag("Starter", data);
            }

            if (this->players[i]["nationality"] != QString()) {
                room->setPlayerProperty(sp, "kingdom", this->players.at(i)["nationality"]);
            }

            str = this->players[i]["draw"];
            if (str == QString()) str = "4";
            room->setTag("FirstRound", true);
            room->drawCards(sp, str.toInt());
            room->setTag("FirstRound", false);

            if (this->players[i]["marks"] != QString()) {
                QStringList marks = this->players[i]["marks"].split(",");
                foreach(QString qs, marks) {
                    QStringList keys = qs.split("*");
                    str = keys[1];
                    room->setPlayerMark(sp, keys[0], str.toInt());
                }
            }
            QString role = HegemonyMode::GetMappedRole(sp->getKingdom());
            room->setPlayerProperty(sp, "role", role);
            QStringList names;
            names.append(sp->getActualGeneral1Name());
            names.append(sp->getActualGeneral2Name());
            room->setTag(sp->objectName(), QVariant::fromValue(names));
        }


        foreach(ServerPlayer *p, room->getAllPlayers()) {
            p->showGeneral(true);
            p->showGeneral(false);
        }

        room->setTag("WaitForPlayer", QVariant(true));
        room->updateStateItem();
        return true;
    }
    else
        return false;
}

void MiniSceneRule::addNPC(QString feature) {
    QMap<QString, QString> player;
    QStringList features;
    if (feature.contains("|"))
        features = feature.split("|");
    else
        features = feature.split(" ");
    foreach(QString str, features) {
        QStringList keys = str.split(":");
        if (keys.size() < 2) continue;
        if (keys.first().size() < 1) continue;
        player.insert(keys.at(0), keys.at(1));
    }

    players << player;
}

void MiniSceneRule::setPile(QString cardList) {
    setup = cardList;
    QStringList cards = setup.split(",", QString::SkipEmptyParts);
    foreach(QString sid, cards) {
        bool ok;
        int id = sid.toInt(&ok);
        Q_ASSERT(ok);
        m_fixedDrawCards.append(id);
    }
}

void MiniSceneRule::setOptions(QStringList option) {
    ex_options[option.first()] = option.last();
}

void MiniSceneRule::loadSetting(QString path) {
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        players.clear();
        setup.clear();

        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString aline = stream.readLine();
            if (aline.isEmpty()) continue;

            if (aline.startsWith("setPile"))
                setPile(aline.split(":").at(1));
            else if (aline.startsWith("extraOptions")) {
                aline.remove("extraOptions:");
                QStringList options = aline.split(" ");
                foreach(QString option, options) {
                    if (options.isEmpty()) continue;
                    QString key = option.split(":").first(), value = option.split(":").last();
                    ex_options[key] = QVariant::fromValue(value);
                }
            }
            else
                addNPC(aline);
        }
        file.close();
    }
}

MiniScene::MiniScene(const QString &name)
    : Scenario(name)
{
    rule = new MiniSceneRule(this);
}

MiniScene::~MiniScene(){
    delete rule;
}

void MiniScene::setupCustom(QString name) const{
    if (name == QString()) name = "custom_scenario";
    name.prepend("etc/customScenes/");
    name.append(".txt");

    MiniSceneRule *arule = qobject_cast<MiniSceneRule *>(this->getRule());
    arule->loadSetting(name);
}

void MiniScene::onTagSet(Room *, const QString &) const{
}

