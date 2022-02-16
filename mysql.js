// const temp = require("./index.js");

var mysql      = require('mysql');

var connection = mysql.createConnection({
  host     : 'localhost',
  user     : 'root',
  password : 'choi2350',
  database : 'testdb1'
});
  
connection.connect();

// CRUD of Table

// C
// var sql = 'CREATE TABLE students(name VARCHAR(20) NOT NULL, data INT NOT NULL, PRIMARY KEY(name))'
var sql = 'CREATE TABLE project(name VARCHAR(20) NOT NULL, data INT NOT NULL)'
connection.query(sql, function(err, rows, fields){
  if(err){
    console.log(err);
  } else {
    console.log(rows);
  }
});

// R
// var sql = 'SELECT * FROM students'
// connection.query(sql, function(err, rows, fields){
//   if(err){
//     console.log(err);
//   } else {
//     console.log(rows);
//   }
// });

// U
// table값을 업데이트 or data값을 업데이트
// var sql = 'RENAME TABLE students to class1'
// var sql = 'ALTER TABLE class1 ADD COLUMN sex INT NOT NULL'

// var sql = 'INSERT INTO class1 VALUES(\'a\',\'10\',\'0\')'
// var sql = 'INSERT INTO class1 VALUES(\'b\',\'10\',\'0\')'
// var sql = 'INSERT INTO class1 VALUES(\'c\',\'10\',\'0\')'

// var sql = 'UPDATE class1 SET sex = 1 WHERE name = \'c\''

// connection.query(sql, function(err, rows, fields){
//   if(err){
//     console.log(err);
//   } else {
//     console.log(rows);
//   }
// });

// 데이터베이스 이름 변경하기 (테스트 안해봄!!)
// 백업
// var sql = 'mysqldump -u root -p class1 > class2.sql'
// 복원
// var sql = 'mysql -u root -p students < class2.sql'

// D
// var sql = 'DELETE FROM class1 WHERE sex = 1'
// var sql = 'DROP TABLE testdb2'
// var sql = 'DROP database testdb1'
// connection.query(sql, function(err, rows, fields){
//   if(err){
//     console.log(err);
//   } else {
//     console.log(rows);
//   }
// });
  
connection.end();

exports.connection = connection;