# mysql总结
## 1. mysql基础

ref: [参考](https://blog.csdn.net/mysnsds/article/details/125313346)

### 1.1 sql语句执行顺序

```sql
（8）Select
（9）distinct 字段名1,字段名2，
（6）[func(字段名)]  
（1）from 表1
（3）<join类型>join 表2 
（2）on <join条件> 
（4）where <where条件> 
（5）group by <字段> 
（7）having <having条件> 
（10）order by <排序字段> 
（11）limit <起始偏移量,行数>
```





## 2. mysql优化建议（技巧）

**扩展**：参考https://www.cnblogs.com/jajian/p/9758192.html

- 整型定义中无需定义显示宽度，比如：使用INT，而不是INT(4)。

- 建议字段定义为`NOT NULL`。

- 对于非Index索引字段作为where条件时，如果确认结果只有一个，可以使用limit 1来提高查询速度。

- 索引中的字段数建议不超过5个。

- 单张表的索引个数控制在5个以内。

- InnoDB表一般都建议有主键列（必须）。

- 建立复合索引时，优先将选择性高的字段放在前面。

- `UPDATE、DELETE`语句需要根据`WHERE`条件添加索引。

- 不建议使用%前缀模糊查询，例如`LIKE “%weibo”`，无法用到索引，会导致全表扫描（但可使用`“weibo%”`）。

- 避免在索引字段上使用函数，否则会导致查询时索引失效。（`select xxx from tab1 where day(DateTime) > 15`）

- 考虑*使用limit N，少用limit M，N*，特别是大表或M比较大的时候。

- SQL语句中IN包含的值不应过多。

- WHERE条件中的字段值需要符合该字段的数据类型，避免MySQL进行隐式类型转化。

- SELECT、INSERT语句必须显式的指明字段名称，禁止使用`SELECT *` 或是`INSERT INTO table_name values()`。

- SQL中尽可能避免反连接，避免半连接，这是优化器做得薄弱的一方面，什么是反连接，半连接？其实比较好理解，举个例子，not in ,not exists就是反连接，in,exists就是半连接，在千万级大表中出现这种问题，性能是几个数量级的差异。

- 尽可能避免或者杜绝多表复杂关联，大表关联是大表处理的噩梦，一旦打开了这个口子，越来越多的需求需要关联，性能优化就没有回头路了，更何况大表关联是MySQL的弱项，尽管Hash Join才推出，不要像掌握了绝对大杀器一样，在商业数据库中早就存在，问题照样层出不穷。

- 尽可能杜绝范围数据的查询，范围扫描在千万级大表情况下还是尽可能减少。

- 不要对字段建立多个索引。

- 使用`explain/desc select`来分析SQL语句执行前的执行计划。

- 使用缓存：避免在查询条件中使用不确定的值。（像now(),datetime()之类的）

**注意**：在查询时，MYSQL只能使用一个索引，如果建立的是多个单列的普通索引，在查询时会根据查询的索引字段，从中选择一个限制最严格的单例索引进行查询。别的索引都不会生效。