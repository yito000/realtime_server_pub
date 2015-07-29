create table identifier (
  table_name varchar(100) not null,
  current_value integer default 1 not null,
  primary key(table_name)
);

create table users (
  id integer not null,
  uiid varchar(64) not null,
  primary key(id)
);

partition table users on column id;

create procedure from class UserSelect;
create procedure from class UserUpdate;
create procedure from class UserList;
create procedure from class UserDeleteAll;
create procedure from class UserAdd;

