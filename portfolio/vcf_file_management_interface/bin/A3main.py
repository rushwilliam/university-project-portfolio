# Name: William Rush
# ID: 1267781

from ctypes import *
from asciimatics.widgets import Frame, ListBox, Layout, Divider, Text, \
    Button, TextBox, Widget, PopUpDialog
from asciimatics.scene import Scene
from asciimatics.screen import Screen
from asciimatics.exceptions import ResizeScreenError, NextScene, StopApplication
import sys
import sqlite3

import os
from datetime import datetime
import mysql.connector

class ContactModel():
    def create(self):
        self.conn.autocommit = True
        self.cursor = self.conn.cursor()
        self.cursor.execute('''
            CREATE TABLE IF NOT EXISTS FILE(
            file_id INT AUTO_INCREMENT PRIMARY KEY,
            file_name VARCHAR(60) NOT NULL,
            last_modified DATETIME,
            creation_time DATETIME NOT NULL
            )
        ''')
        self.cursor.execute('''
            CREATE TABLE IF NOT EXISTS CONTACT(
            contact_id INT AUTO_INCREMENT PRIMARY KEY,
            name VARCHAR(256) NOT NULL,
            birthday DATETIME,
            anniversary DATETIME,
            file_id INT NOT NULL,
            FOREIGN KEY (file_id) REFERENCES FILE(file_id) ON DELETE CASCADE
            )
        ''')
        file_list = vclib.getCardList().decode("utf-8").splitlines()
        for file_name in file_list:
            self.cursor.execute(f'''
                SELECT *
                FROM FILE
                WHERE file_name = '{file_name}'
            ''')
            if (self.cursor.fetchone()):
                continue
            self.cursor.execute(f'''
                INSERT INTO FILE VALUES(
                NULL,
                '{file_name}',
                '{datetime.fromtimestamp(os.path.getmtime(f'cards/{file_name}'))}',
                NOW()
                )
            ''')
            contact_info = vclib.getContactInfo(file_name.encode("utf-8")).decode("utf-8").splitlines()
            birthday = "NULL" if len(contact_info[1]) == 0 else f"'{contact_info[1]}'"
            anniversary = "NULL" if len(contact_info[2]) == 0 else f"'{contact_info[2]}'"
            self.cursor.execute(f'''
                SELECT file_id
                FROM FILE
                WHERE file_name = '{file_name}'
            ''')
            file_id = self.cursor.fetchone()[0]
            self.cursor.execute(f'''
                INSERT INTO CONTACT VALUES(
                NULL,
                '{contact_info[0]}',
                {birthday},
                {anniversary},
                {file_id}
                )
            ''')
        # Current contact when editing.
        self.current_id = None
    
    def login(self, details):
        try:
            self.conn = mysql.connector.connect(host="dursley.socs.uoguelph.ca",
                                database=details["dbname"],
                                user=details["username"],
                                password=details["password"])
        except mysql.connector.Error as err:
            return False
        self.create()
        return True

    def add(self, contact):
        self.cursor.execute(f'''
            INSERT INTO FILE VALUES(
            NULL,
            '{contact["file_name"]}',
            NOW(),
            NOW()
            )
        ''')
        self.cursor.execute(f'''
            SELECT file_id
            FROM FILE
            WHERE file_name = '{contact["file_name"]}'
        ''')
        file_id = self.cursor.fetchone()[0]
        self.cursor.execute(f'''
            INSERT INTO CONTACT VALUES(
            NULL,
            '{contact["contact"]}',
            NULL,
            NULL,
            {file_id}
            )
        ''')
        vclib.writeNewCard(contact["file_name"].encode("utf-8"), contact["contact"].encode("utf-8"))

    def get_summary(self):
        self.cursor.execute(f'''
            SELECT file_name, file_id
            FROM FILE
        ''')
        return self.cursor.fetchall()

    def get_contact(self, contact_id):
        self.cursor.execute(f'''
            SELECT file_name, name
            FROM (
                FILE INNER JOIN CONTACT
                ON CONTACT.file_id = FILE.file_id
            )
            WHERE FILE.file_id = {contact_id}
        ''')
        data = self.cursor.fetchone()
        dates = vclib.getFormattedDates(data[0].encode("utf-8")).decode("utf-8").splitlines()
        return {
            "file_name": data[0],
            "contact": data[1],
            "birthday": dates[0],
            "anniversary": dates[1],
            "other_properties": str(vclib.countOptionalProperties(data[0].encode("utf-8")))
        }

    def get_current_contact(self):
        if self.current_id is None:
            return {"file_name": "", "contact": "", "birthday": "", "anniversary": "", "other_properties": ""}
        else:
            return self.get_contact(self.current_id)

    def update_current_contact(self, details):
        if self.current_id is None:
            # Only time add is called
            self.add(details)
        else:
            self.cursor.execute(f'''
                SELECT file_id
                FROM FILE
                WHERE file_name = '{details["file_name"]}'
            ''')
            file_id = self.cursor.fetchone()[0]
            self.cursor.execute(f'''
                UPDATE CONTACT
                SET name = '{details["contact"]}'
                WHERE file_id = '{file_id}'
            ''')
            vclib.updateContact(details["file_name"].encode("utf-8"), details["contact"].encode("utf-8"))

    def get_all_contacts(self):
        self.cursor.execute('''
            SELECT FILE.file_name, CONTACT.name, CONTACT.birthday, CONTACT.anniversary
            FROM FILE INNER JOIN CONTACT ON FILE.file_id = CONTACT.file_id
            ORDER BY CONTACT.name
        ''')
        output = self.cursor.fetchall()
        return [(f"{output[i][0]} {output[i][1]} {output[i][2]} {output[i][3]}", 0) for i in range(len(output))]

    def get_all_born_in_june(self):
        self.cursor.execute('''
            SELECT FILE.file_name, CONTACT.birthday
            FROM FILE INNER JOIN CONTACT ON FILE.file_id = CONTACT.file_id
            WHERE MONTH(CONTACT.birthday) = 6
            ORDER BY DATEDIFF(CONTACT.birthday, FILE.last_modified)
        ''')
        output = self.cursor.fetchall()
        return [(f"{output[i][0]} {output[i][1]}", 0) for i in range(len(output))]
    # def delete_contact(self, contact_id):
    #     self._db.cursor().execute('''
    #         DELETE FROM contacts WHERE id=:id''', {"id": contact_id})
    #     self._db.commit()

    def count_records(self):
        count = []
        self.cursor.execute('''
            SELECT * FROM FILE
        ''')
        count.append(len(self.cursor.fetchall()))
        self.cursor.execute('''
            SELECT * FROM CONTACT
        ''')
        count.append(len(self.cursor.fetchall()))
        return count

class LoginView(Frame):
    def __init__(self, screen, model):
        super(LoginView, self).__init__(screen,
                                        screen.height * 2 // 3,
                                        screen.width * 2 // 3,
                                        hover_focus=True,
                                        can_scroll=False,
                                        title="Login",
                                        reduce_cpu=True)
     
        self._model = model

        layout = Layout([100], fill_frame=True)
        self.add_layout(layout)
        layout.add_widget(Text("Username:", "username"))
        layout.add_widget(Text("Password:", "password"))
        layout.add_widget(Text("DB name:", "dbname"))
        layout2 = Layout([1, 1, 1, 1])
        self.add_layout(layout2)
        layout2.add_widget(Button("OK", self._ok), 0)
        layout2.add_widget(Button("Cancel", self._cancel), 3)
        self.fix()
    
    def _ok(self):
        self.save()
        if self._model.login(self.data):
            raise NextScene("Main")
        else:
            self._scene.add_effect(PopUpDialog(self.screen, "ERROR: Could not connect to the database", ["OK"]))

    @staticmethod
    def _cancel():
        raise StopApplication("User pressed quit")


class ListView(Frame):
    def __init__(self, screen, model):
        super(ListView, self).__init__(screen,
                                       screen.height * 2 // 3,
                                       screen.width * 2 // 3,
                                       on_load=self._reload_list,
                                       hover_focus=True,
                                       can_scroll=False,
                                       title="Contact List")
        # Save off the model that accesses the contacts database.
        self._model = model

        # Create the form for displaying the list of contacts.
        self._list_view = ListBox(
            Widget.FILL_FRAME,
            # model.get_summary(),
            "",
            name="contacts",
            add_scroll_bar=True,
            on_change=self._on_pick,
            on_select=self._edit)
        layout = Layout([100], fill_frame=True)
        self.add_layout(layout)
        layout.add_widget(self._list_view)
        layout.add_widget(Divider())
        layout2 = Layout([1, 1, 1, 1])
        self.add_layout(layout2)
        layout2.add_widget(Button("Create", self._create), 0)
        self._edit_button = Button("Edit", self._edit)
        self._delete_button = Button("DB queries", self._dbqueries)
        layout2.add_widget(self._edit_button, 1)
        layout2.add_widget(self._delete_button, 2)
        layout2.add_widget(Button("Quit", self._quit), 3)
        self.fix()
        self._on_pick()

    def _on_pick(self):
        self._edit_button.disabled = self._list_view.value is None
        self._delete_button.disabled = self._list_view.value is None

    def _reload_list(self, new_value=None):
        self._list_view.options = self._model.get_summary()
        self._list_view.value = new_value

    def _create(self):
        self._model.current_id = None
        raise NextScene("Edit Contact")

    def _edit(self):
        self.save()
        self._model.current_id = self.data["contacts"]
        raise NextScene("Edit Contact")

    def _dbqueries(self):
        raise NextScene("DBQueries")
        

    @staticmethod
    def _quit():
        raise StopApplication("User pressed quit")


class ContactView(Frame):
    def __init__(self, screen, model):
        super(ContactView, self).__init__(screen,
                                          screen.height * 2 // 3,
                                          screen.width * 2 // 3,
                                          hover_focus=True,
                                          can_scroll=False,
                                          title="Contact Details",
                                          reduce_cpu=True)
        # Save off the model that accesses the contacts database.
        self._model = model

        # Create the form for displaying the list of contacts.
        layout = Layout([100], fill_frame=True)
        self.add_layout(layout)
        self._file_name_text = Text("File name:", "file_name")
        layout.add_widget(self._file_name_text)
        layout.add_widget(Text("Contact:", "contact"))
        layout.add_widget(Text("Birthday:", "birthday", readonly=True))
        layout.add_widget(Text("Anniversary:", "anniversary", readonly=True))
        layout.add_widget(Text("Other properties:", "other_properties", readonly=True))
        layout2 = Layout([1, 1, 1, 1])
        self.add_layout(layout2)
        layout2.add_widget(Button("OK", self._ok), 0)
        layout2.add_widget(Button("Cancel", self._cancel), 3)
        self.fix()

    def reset(self):
        # Do standard reset to clear out form, then popul ate with new data.
        super(ContactView, self).reset()
        self._file_name_text.readonly = self._model.current_id is not None
        self.data = self._model.get_current_contact()

    def _ok(self):
        self.save()
        if len(self.data["file_name"]) == 0:
            self._scene.add_effect(PopUpDialog(self.screen, "ERROR: Please enter a file name", ["OK"]))
            return
        if self.data["file_name"][-4:] != ".vcf" and self.data["file_name"][-6:] != ".vcard":
            self._scene.add_effect(PopUpDialog(self.screen, "ERROR: Invalid file extension", ["OK"]))
            return
        if len(self.data["file_name"].split(".")[0]) == 0:
            self._scene.add_effect(PopUpDialog(self.screen, "ERROR: File name cannot be empty", ["OK"]))
            return
        if len(self.data["contact"]) == 0:
            self._scene.add_effect(PopUpDialog(self.screen, "ERROR: Please enter a contact name", ["OK"]))
            return
        self._model.update_current_contact(self.data)
        raise NextScene("Main")

    @staticmethod
    def _cancel():
        raise NextScene("Main")

class DBQueries(Frame):
    def __init__(self, screen, model):
        super(DBQueries, self).__init__(screen,
                                        screen.height * 2 // 3,
                                        screen.width * 2 // 3,
                                        hover_focus=True,
                                        can_scroll=False,
                                        title="DB Queries",
                                        reduce_cpu=True)
        self._model = model
        self._output = ListBox(
            Widget.FILL_FRAME,
            "",
            name="contacts",
            add_scroll_bar=True
        )
        layout = Layout([100], fill_frame=True)
        self.add_layout(layout)
        layout.add_widget(self._output)
        layout.add_widget(Divider())

        layout2 = Layout([1, 1, 1])
        self.add_layout(layout2)
        layout2.add_widget(Button("Display all contacts", self._display_all), 0)
        layout2.add_widget(Button("Find contact born in June", self._born_in_june), 1)
        layout2.add_widget(Button("Cancel", self._cancel), 2)
        self.fix()

    def _display_all(self):
        self._output.options = self._model.get_all_contacts()
        count = self._model.count_records()
        self._scene.add_effect(PopUpDialog(self.screen, f"Database has {count[0]} files and {count[0]} contacts", ["OK"]))

    def _born_in_june(self):
        self._output.options = self._model.get_all_born_in_june()
        count = self._model.count_records()
        self._scene.add_effect(PopUpDialog(self.screen, f"Database has {count[0]} files and {count[0]} contacts", ["OK"]))
    
    def _cancel(self):
        raise NextScene("Main")


def demo(screen, scene):
    scenes = [
        Scene([LoginView(screen, contacts)], -1, name="Login"),
        Scene([ListView(screen, contacts)], -1, name="Main"),
        Scene([ContactView(screen, contacts)], -1, name="Edit Contact"),
        Scene([DBQueries(screen, contacts)], -1, name="DBQueries")
    ]

    screen.play(scenes, stop_on_resize=True, start_scene=scene, allow_int=True)

vclib = CDLL('./libvcparser.so')
vclib.getCardList.argtypes = []
vclib.getCardList.restype = c_char_p
vclib.getContactInfo.argtypes = [c_char_p]
vclib.getContactInfo.restype = c_char_p
vclib.getFormattedDates.argtypes = [c_char_p]
vclib.getFormattedDates.restype = c_char_p
vclib.writeNewCard.argtypes = [c_char_p, c_char_p]
vclib.writeNewCard.restype = None
vclib.updateContact.argtypes = [c_char_p, c_char_p]
vclib.updateContact.restype = None
vclib.countOptionalProperties.argtypes = [c_char_p]
vclib.countOptionalProperties.restype = c_int


contacts = ContactModel()
last_scene = None
while True:
    try:
        Screen.wrapper(demo, catch_interrupt=True, arguments=[last_scene])
        sys.exit(0)
    except ResizeScreenError as e:
        last_scene = e.scene