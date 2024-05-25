INSERT INTO houses (house_name, head_name)
SELECT DISTINCT house, head FROM students;

INSERT INTO students_new
SELECT id, student_name FROM students;

INSERT INTO assignments
SELECT students.id, houses.house_id FROM students JOIN houses ON students.house = houses.house_name;

DROP TABLE students;
ALTER TABLE students_new
    RENAME TO students;
