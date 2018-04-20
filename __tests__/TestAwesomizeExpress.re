open Jest;

type t = {clientId: int};

let req: Express.Request.t = [%raw "{\"clientId\": \"1\"}"];

let res: Express.Response.t = [%raw
  "{ status: () => ({ json: () => ({\"clientId\": \"1\"}) }) }"
];

external complete : Express.complete => t = "%identity";

external encoded : Express.Request.t => Js.Json.t = "%identity";

describe("Awesomize Express", () =>
  describe("make", () => {
    let schema = [|
      (
        "clientId",
        {
          read: Awesomize.Read.key("clientId"),
          sanitize: None,
          validate: [Awesomize.Validator.required],
          normalize: None,
        }: Awesomize.definition,
      ),
    |];
    let decoder = _ => {clientId: 1};
    let handler = (req, _) => Js.Promise.resolve @@ encoded @@ req;
    let encoder = x => x;
    testPromise("success", () =>
      AwesomizeExpress.make(
        ~schema,
        ~decoder,
        ~encoder,
        ~handler,
        (),
        req,
        res,
      )
      |> Js.Promise.then_(result =>
           (
             switch (complete @@ result) {
             | {clientId} => pass
             | _ => fail("failed")
             }
           )
           |> Js.Promise.resolve
         )
    );
  })
);
