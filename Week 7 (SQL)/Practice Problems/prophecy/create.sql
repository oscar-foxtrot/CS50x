CREATE TABLE houses (
    house_id INTEGER,
    house_name TEXT,
    head_name TEXT,
    PRIMARY KEY (house_id)
);

CREATE TABLE students_new (
    student_id INTEGER,
    student_name TEXT,
    PRIMARY KEY (student_id)
);

CREATE TABLE assignments (
    student_id INTEGER,
    house_id INTEGER,
    FOREIGN KEY (student_id) REFERENCES students_new(student_id),
    FOREIGN KEY (house_id) REFERENCES houses(house_id)
);
